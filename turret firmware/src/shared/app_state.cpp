#include "shared/app_state.h"

NimBLEServer *pServer = nullptr;
Servo turretMotor;
Servo headServo;
VL53L4CX tof_sensor;
CRGB leds[constants::ledAmount];
volatile turretModes currentMode = LOCK;
WiFiUDP cameraUdp;
IPAddress remoteIp(192, 168, 1, 100);
