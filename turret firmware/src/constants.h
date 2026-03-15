#ifndef INF_TURRET_CONSTANTS_H
#define INF_TURRET_CONSTANTS_H
#include "FastLED.h"
#include "detection/hsv_utils.h"

namespace constants {
    HsvPixel dog_min_hsv(50 / 3, 20, 20);
    HsvPixel dog_max_hsv(40 / 3, 127, 127); // my example change it as you wish

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
