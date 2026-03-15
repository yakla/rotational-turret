#include "hsv_utils.h"

#include <Arduino.h>

static uint8_t expand5To8(const uint8_t value) {
    return static_cast<uint8_t>((value << 3) | (value >> 2));
}

static uint8_t expand6To8(const uint8_t value) {
    return static_cast<uint8_t>((value << 2) | (value >> 4));
}

HsvPixel rgb565ToHsv(const uint16_t pixel) {
    const uint8_t r = expand5To8(static_cast<uint8_t>((pixel >> 11) & 0x1F));
    const uint8_t g = expand6To8(static_cast<uint8_t>((pixel >> 5) & 0x3F));
    const uint8_t b = expand5To8(static_cast<uint8_t>(pixel & 0x1F));

    const uint8_t cmax = max(r, max(g, b));
    const uint8_t cmin = min(r, min(g, b));
    const uint8_t delta = cmax - cmin;

    uint8_t h = 0;
    if (delta != 0) {
        int16_t hue = 0;
        if (cmax == r) {
            hue = static_cast<int8_t>((60L * (static_cast<int16_t>(g) - static_cast<int16_t>(b))) / delta);
        } else if (cmax == g) {
            hue = static_cast<int8_t>(120 + (60L * (static_cast<int16_t>(b) - static_cast<int16_t>(r))) / delta);
        } else {
            hue = static_cast<int8_t>(240 + (60L * (static_cast<int16_t>(r) - static_cast<int16_t>(g))) / delta);
        }

        if (hue < 0) hue += 360;
        if (hue >= 360) hue -= 360;
        h = static_cast<uint8_t>(hue);
    }

    const uint8_t s = cmax == 0 ? 0 : static_cast<uint8_t>((static_cast<uint16_t>(delta) * 255U) / cmax);
    return {h, s, cmax};
}

static bool hueInRange(uint16_t h, uint16_t minH, uint16_t maxH) {
    if (minH <= maxH) {
        return h >= minH && h <= maxH;
    }
    return h >= minH || h <= maxH;
}

bool hsvMatchesRange(const HsvPixel& hsv, const HsvPixel& minHsv, const HsvPixel& maxHsv) {
    return hueInRange(hsv.h, minHsv.h, maxHsv.h) &&
           hsv.s >= minHsv.s && hsv.s <= maxHsv.s &&
           hsv.v >= minHsv.v && hsv.v <= maxHsv.v;
}
