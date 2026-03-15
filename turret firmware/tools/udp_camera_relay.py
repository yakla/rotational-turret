#!/usr/bin/env python3
"""Receive JPEG fragments over UDP and relay the latest frame over HTTP."""

from __future__ import annotations

import argparse
import socket
import sys
import threading
import time
from dataclasses import dataclass, field
from http import server
from typing import Optional

try:
    import cv2  # type: ignore
    import numpy as np  # type: ignore
except ImportError:
    cv2 = None
    np = None


JPEG_START = b"\xff\xd8"
JPEG_END = b"\xff\xd9"


@dataclass
class SharedFrame:
    frame: Optional[bytes] = None
    frame_count: int = 0
    dropped_bytes: int = 0
    last_sender: Optional[tuple[str, int]] = None
    updated_at: float = 0.0
    condition: threading.Condition = field(default_factory=threading.Condition)

    def update(self, frame: bytes, sender: tuple[str, int]) -> None:
        with self.condition:
            self.frame = frame
            self.frame_count += 1
            self.last_sender = sender
            self.updated_at = time.time()
            self.condition.notify_all()

    def snapshot(self) -> tuple[Optional[bytes], int, float, Optional[tuple[str, int]]]:
        with self.condition:
            return self.frame, self.frame_count, self.updated_at, self.last_sender


class JpegAssembler:
    def __init__(self, max_buffer_size: int) -> None:
        self.buffer = bytearray()
        self.max_buffer_size = max_buffer_size

    def push(self, packet: bytes) -> list[bytes]:
        self.buffer.extend(packet)
        frames: list[bytes] = []

        while True:
            start = self.buffer.find(JPEG_START)
            if start == -1:
                self._trim_tail()
                return frames

            if start > 0:
                del self.buffer[:start]

            end = self.buffer.find(JPEG_END, 2)
            if end == -1:
                self._trim_head()
                return frames

            frames.append(bytes(self.buffer[: end + 2]))
            del self.buffer[: end + 2]

    def _trim_tail(self) -> None:
        if len(self.buffer) > self.max_buffer_size:
            del self.buffer[:-2]

    def _trim_head(self) -> None:
        if len(self.buffer) > self.max_buffer_size:
            keep_from = max(0, len(self.buffer) - self.max_buffer_size)
            del self.buffer[:keep_from]


class RelayHttpHandler(server.BaseHTTPRequestHandler):
    shared_frame: SharedFrame

    def do_GET(self) -> None:  # noqa: N802 - BaseHTTPRequestHandler API
        if self.path in ("/", "/index.html"):
            self._serve_index()
            return

        if self.path == "/stream.mjpg":
            self._serve_stream()
            return

        if self.path == "/healthz":
            self._serve_health()
            return

        self.send_error(404, "Not found")

    def log_message(self, format: str, *args: object) -> None:
        sys.stderr.write("%s - - [%s] %s\n" % (self.address_string(), self.log_date_time_string(), format % args))

    def _serve_index(self) -> None:
        body = (
            "<!doctype html><html><head><title>Turret Camera</title></head>"
            "<body style='background:#111;color:#eee;font-family:sans-serif'>"
            "<h1>Turret Camera</h1>"
            "<img src='/stream.mjpg' style='max-width:100%;height:auto' />"
            "</body></html>"
        ).encode("utf-8")
        self.send_response(200)
        self.send_header("Content-Type", "text/html; charset=utf-8")
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def _serve_health(self) -> None:
        frame, frame_count, updated_at, sender = self.shared_frame.snapshot()
        age = -1.0 if updated_at == 0 else time.time() - updated_at
        body = (
            f"frames={frame_count}\n"
            f"has_frame={int(frame is not None)}\n"
            f"age_seconds={age:.3f}\n"
            f"sender={sender}\n"
        ).encode("utf-8")
        self.send_response(200)
        self.send_header("Content-Type", "text/plain; charset=utf-8")
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def _serve_stream(self) -> None:
        boundary = "frame"
        self.send_response(200)
        self.send_header("Cache-Control", "no-store, no-cache, must-revalidate")
        self.send_header("Pragma", "no-cache")
        self.send_header("Connection", "close")
        self.send_header("Content-Type", f"multipart/x-mixed-replace; boundary={boundary}")
        self.end_headers()

        last_sent = 0
        try:
            while True:
                with self.shared_frame.condition:
                    self.shared_frame.condition.wait_for(lambda: self.shared_frame.frame_count > last_sent, timeout=5.0)
                    frame = self.shared_frame.frame
                    frame_count = self.shared_frame.frame_count

                if frame is None or frame_count == last_sent:
                    continue

                last_sent = frame_count
                self.wfile.write(
                    (
                        f"--{boundary}\r\n"
                        "Content-Type: image/jpeg\r\n"
                        f"Content-Length: {len(frame)}\r\n\r\n"
                    ).encode("ascii")
                )
                self.wfile.write(frame)
                self.wfile.write(b"\r\n")
        except (BrokenPipeError, ConnectionResetError):
            return


class ThreadingHttpServer(server.ThreadingHTTPServer):
    daemon_threads = True


def udp_receiver(shared_frame: SharedFrame, host: str, port: int, packet_size: int, max_buffer_size: int) -> None:
    assembler = JpegAssembler(max_buffer_size=max_buffer_size)
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((host, port))
    print(f"[udp] listening on {host}:{port}")

    while True:
        packet, sender = sock.recvfrom(packet_size)
        for frame in assembler.push(packet):
            shared_frame.update(frame, sender)


def preview_loop(shared_frame: SharedFrame) -> None:
    if cv2 is None or np is None:
        print("[preview] OpenCV not installed, skipping preview window")
        return

    last_seen = 0
    while True:
        with shared_frame.condition:
            shared_frame.condition.wait_for(lambda: shared_frame.frame_count > last_seen)
            frame = shared_frame.frame
            last_seen = shared_frame.frame_count

        if frame is None:
            continue

        image = cv2.imdecode(np.frombuffer(frame, dtype=np.uint8), cv2.IMREAD_COLOR)
        if image is None:
            continue

        cv2.imshow("Turret UDP Camera", image)
        if cv2.waitKey(1) & 0xFF == 27:
            break

    cv2.destroyAllWindows()


def build_arg_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--udp-host", default="0.0.0.0", help="UDP bind host")
    parser.add_argument("--udp-port", type=int, default=12345, help="UDP bind port")
    parser.add_argument("--packet-size", type=int, default=2048, help="Maximum UDP packet size to read")
    parser.add_argument("--http-host", default="0.0.0.0", help="HTTP bind host")
    parser.add_argument("--http-port", type=int, default=8080, help="HTTP bind port")
    parser.add_argument(
        "--max-buffer-size",
        type=int,
        default=512 * 1024,
        help="Maximum buffered bytes while searching for JPEG markers",
    )
    parser.add_argument("--no-http", action="store_true", help="Disable HTTP relay")
    parser.add_argument("--preview", action="store_true", help="Open a local preview window with OpenCV")
    return parser


def main() -> int:
    args = build_arg_parser().parse_args()
    shared_frame = SharedFrame()

    receiver_thread = threading.Thread(
        target=udp_receiver,
        args=(shared_frame, args.udp_host, args.udp_port, args.packet_size, args.max_buffer_size),
        daemon=True,
    )
    receiver_thread.start()

    if args.preview:
        preview_thread = threading.Thread(target=preview_loop, args=(shared_frame,), daemon=True)
        preview_thread.start()

    if args.no_http:
        receiver_thread.join()
        return 0

    RelayHttpHandler.shared_frame = shared_frame
    httpd = ThreadingHttpServer((args.http_host, args.http_port), RelayHttpHandler)
    print(f"[http] serving http://{args.http_host}:{args.http_port}")
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        print("\n[http] shutting down")
    finally:
        httpd.server_close()

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
