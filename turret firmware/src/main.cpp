#include <Arduino.h>
#include <ESP32Servo.h>
#include <esp_camera.h>
#include <FastLED.h>
#include <WiFi.h>
#include <Wire.h>

#include "secret.h"
#include "shared/app_state.h"
#include "ble/ble_control.h"
#include "camera/camera_stream.h"
#include "led/led_control.h"

#define CAMERA_UDP_PORT 12345


void colorPipeLineDetection();

void setup() {
    Serial.begin(115200);

    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFiClass::status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected");
    Serial.println(WiFi.localIP());

    CFastLED::addLeds<NEOPIXEL, constants::LED_DATA>(leds, constants::ledAmount);
    FastLED.setBrightness(70);
    Wire.begin(constants::TOF_SDA, constants::TOF_SCL);
    tof_sensor = VL53L4CX(&Wire, constants::TOF_XSHUT);

    turretMotor.attach(constants::MOTOR, 1000, 2000);
    headServo.attach(constants::SERVO, 0, 2048);

    setupBleControl();
    xTaskCreatePinnedToCore(
        LedControl,
        "LED_CONTROL",
        2048,
        nullptr,
        1,
        nullptr,
        0
    );

    if (setupCamera()) {
        cameraUdp.begin(CAMERA_UDP_PORT);
        xTaskCreatePinnedToCore(
            udp_stream_task, "udp_stream",
            8192,
            nullptr
            , 1,
            nullptr
            , 1);
    } else {
        Serial.println("Camera setup failed");
    }
}


void loop() {
    if (!remoteIp) ip_listener();
}
