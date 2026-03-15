#pragma once

#include <ESP32Servo.h>
#include <FastLED.h>
#include <NimBLEDevice.h>
#include <WiFiUdp.h>

#include "constants.h"
#include "vl53l4cx_class.h"

enum turretModes {
    MANUAL_CONTROL = 0,
    FOLLOWER = 1,
    LOCK
};

extern NimBLEServer *pServer;
extern Servo turretMotor;
extern Servo headServo;
extern VL53L4CX tof_sensor;
extern CRGB leds[constants::ledAmount];
extern volatile turretModes currentMode;
extern WiFiUDP cameraUdp;
extern IPAddress remoteIp;
