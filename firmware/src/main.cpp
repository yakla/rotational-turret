#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

// WiFi credentials – define these in a separate secrets.h file (see secrets.h.example)
#include "secrets.h"

// Servo pins
#define PAN_SERVO_PIN  D1
#define TILT_SERVO_PIN D2

Servo panServo;
Servo tiltServo;

ESP8266WebServer server(80);

int panAngle  = 90;
int tiltAngle = 90;

void handleRoot() {
  String html = R"(
<!DOCTYPE html>
<html>
<head><title>Rotational Turret</title></head>
<body>
  <h1>Rotational Turret Control</h1>
  <p>Pan: <input type='range' min='0' max='180' value='90'
       oninput="fetch('/pan?angle='+this.value)"></p>
  <p>Tilt: <input type='range' min='0' max='180' value='90'
       oninput="fetch('/tilt?angle='+this.value)"></p>
</body>
</html>
)";
  server.send(200, "text/html", html);
}

void handlePan() {
  if (server.hasArg("angle")) {
    panAngle = server.arg("angle").toInt();
    panAngle = constrain(panAngle, 0, 180);
    panServo.write(panAngle);
  }
  server.send(200, "text/plain", "OK");
}

void handleTilt() {
  if (server.hasArg("angle")) {
    tiltAngle = server.arg("angle").toInt();
    tiltAngle = constrain(tiltAngle, 0, 180);
    tiltServo.write(tiltAngle);
  }
  server.send(200, "text/plain", "OK");
}

void setup() {
  Serial.begin(115200);

  panServo.attach(PAN_SERVO_PIN);
  tiltServo.attach(TILT_SERVO_PIN);
  panServo.write(panAngle);
  tiltServo.write(tiltAngle);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/",     handleRoot);
  server.on("/pan",  handlePan);
  server.on("/tilt", handleTilt);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
