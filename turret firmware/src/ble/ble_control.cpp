#include "ble_control.h"

#include <Arduino.h>
#include <NimBLEDevice.h>

#include "shared/app_state.h"

#define SERVICE_UUID "4cad297a1b0143df836d80c718cf3cd1"
#define DEVICE_NAME "TurretControl"

class RotationCallbacks : public NimBLECharacteristicCallbacks {
    void onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) override {
        const std::string value = pCharacteristic->getValue();
        if (value.length() < 3) return;

        switch (value[2]) {
            case MANUAL_CONTROL:
                currentMode = MANUAL_CONTROL;
                turretMotor.writeMicroseconds(map(value[0], 0, 127, 1000, 2000));
                headServo.writeMicroseconds(map(value[1], 0, 127, 0, 2000));
                break;
            case FOLLOWER:
                currentMode = FOLLOWER;
                break;
            default:
                currentMode = LOCK;
                turretMotor.writeMicroseconds(1500);
                headServo.writeMicroseconds(1000);
                break;
        }
    }

    void onSubscribe(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo, uint16_t subValue) override {
        Serial.println("subscribe");
    }
};

void setupBleControl() {
    NimBLEDevice::init(DEVICE_NAME);
    pServer = NimBLEDevice::createServer();

    NimBLEService *pService = pServer->createService(SERVICE_UUID);
    NimBLECharacteristic *pRotationCharacteristic = pService->createCharacteristic("1234");
    pRotationCharacteristic->setCallbacks(new RotationCallbacks);

    pService->start();

    NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setName(DEVICE_NAME);
    pAdvertising->start();
}
