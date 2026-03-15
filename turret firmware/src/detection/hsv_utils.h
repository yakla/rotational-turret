#pragma once

#include <stdint.h>

struct HsvPixel {
    uint8_t h;
    uint8_t s;
    uint8_t v;

    HsvPixel(const uint8_t h, const uint8_t s, const uint8_t v) : h(h), s(s), v(v) {
    }
};

HsvPixel rgb565ToHsv(uint16_t pixel);

bool hsvMatchesRange(const HsvPixel &hsv, const HsvPixel &minHsv, const HsvPixel &maxHsv);
