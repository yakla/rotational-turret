#include "camera_stream.h"

#include <Arduino.h>
#include <WiFiUdp.h>

#include "esp_camera.h"
#include "detection/color_pipeline.h"

#include "shared/app_state.h"

#define CAMERA_UDP_PORT 12345
#define UDP_PACKET_SIZE 1436

bool setupCamera() {
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = 15;
    config.pin_d1 = 17;
    config.pin_d2 = 18;
    config.pin_d3 = 16;
    config.pin_d4 = 14;
    config.pin_d5 = 12;
    config.pin_d6 = 11;
    config.pin_d7 = 48;
    config.pin_xclk = 10;
    config.pin_pclk = 13;
    config.pin_vsync = 38;
    config.pin_href = 47;
    config.pin_sccb_sda = 40;
    config.pin_sccb_scl = 39;
    config.pin_pwdn = -1;
    config.pin_reset = -1;
    config.xclk_freq_hz = 20000000;
    config.frame_size = FRAMESIZE_QVGA;
    config.pixel_format = PIXFORMAT_JPEG;
    config.grab_mode = CAMERA_GRAB_LATEST;
    config.fb_location = CAMERA_FB_IN_PSRAM;
    config.jpeg_quality = 15;
    config.fb_count = 2;

    const esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        return false;
    }
    return true;
}

void ip_listener() {
    if (!cameraUdp.parsePacket()) return;
    remoteIp = cameraUdp.remoteIP();
    Serial.print("Received packet from: ");
    Serial.println(remoteIp);
}

[[noreturn]] void udp_stream_task(void *pvParameters) {
    while (true) {
        static turretModes lastMode = LOCK;

        if (currentMode != lastMode) {
            sensor_t *sensor = esp_camera_sensor_get();
            if (currentMode == FOLLOWER) {
                sensor->set_framesize(sensor, FRAMESIZE_QQVGA);
                sensor->set_pixformat(sensor, PIXFORMAT_RGB565);
            } else {
                sensor->set_framesize(sensor, FRAMESIZE_QVGA);
                sensor->set_pixformat(sensor, PIXFORMAT_JPEG);
                sensor->set_quality(sensor, 12);
            }
            lastMode = currentMode;
        }

        if (currentMode == FOLLOWER) {
            vTaskDelay(20 / portTICK_PERIOD_MS);
            continue;
        }

        camera_fb_t *fb = esp_camera_fb_get();

        if (!fb) {
            Serial.println("Camera capture failed");
            vTaskDelay(100 / portTICK_PERIOD_MS);
            continue;
        }

        if (fb->format != PIXFORMAT_JPEG) {
            colorPipeLineDetection(fb);
        }

        const size_t size = fb->len;
        const uint8_t *buffer = fb->buf;
        const uint16_t packet_num = (size + UDP_PACKET_SIZE - 1) / UDP_PACKET_SIZE;

        for (uint16_t i = 0; i < packet_num; i++) {
            size_t chunk_size = (i == packet_num - 1) ? (size % UDP_PACKET_SIZE) : UDP_PACKET_SIZE;
            if (chunk_size == 0 && i == packet_num - 1) chunk_size = UDP_PACKET_SIZE;

            cameraUdp.beginPacket(remoteIp, CAMERA_UDP_PORT);
            cameraUdp.write(buffer + (i * UDP_PACKET_SIZE), chunk_size);
            cameraUdp.endPacket();
        }

        esp_camera_fb_return(fb);
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
}
