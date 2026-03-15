#ifndef INF_TURRET_CONSTANTS_H
#define INF_TURRET_CONSTANTS_H
#include "FastLED.h"

namespace constants {
    enum Pins {
        MOTOR = 3,
        SERVO = 2,
        LED = 9,
        TOF_SCL = 6,
        TOF_SDA = 5,
        TOF_GPIO = 7,
        TOF_XSHUT = 4,
        LED_DATA = 9
    };
    constexpr int ledAmount = 12;
    constexpr uint8_t verticalFov = 43;
    constexpr uint8_t horizontalFov = 56;

    extern const CRGBPalette16 purplePalette;
    extern const CHSVPalette16 bluePalette;


}

#endif //INF_TURRET_CONSTANTS_H
