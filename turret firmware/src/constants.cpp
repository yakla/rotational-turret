#include "constants.h"

namespace constants {
    const CRGBPalette16 purplePalette(
        CRGB::DarkViolet,
        CRGB::DarkViolet,
        CRGB::DarkViolet,
        CRGB::DarkViolet,
        CRGB::Magenta,
        CRGB::Magenta,
        CRGB::Linen,
        CRGB::Linen,
        CRGB::Magenta,
        CRGB::Magenta,
        CRGB::DarkViolet,
        CRGB::DarkViolet,
        CRGB::DarkViolet,
        CRGB::DarkViolet,
        CRGB::Linen,
        CRGB::Linen
    );

    const CHSVPalette16 bluePalette(
        CHSV(213, 80, 100 - (100 / 15) * 0),
        CHSV(213, 80, 100 - (100 / 15) * 1),
        CHSV(213, 80, 100 - (100 / 15) * 2),
        CHSV(213, 80, 100 - (100 / 15) * 3),
        CHSV(213, 80, 100 - (100 / 15) * 4),
        CHSV(213, 80, 100 - (100 / 15) * 5),
        CHSV(213, 80, 100 - (100 / 15) * 6),
        CHSV(213, 80, 100 - (100 / 15) * 7),
        CHSV(213, 80, 100 - (100 / 15) * 8),
        CHSV(213, 80, 100 - (100 / 15) * 9),
        CHSV(213, 80, 100 - (100 / 15) * 10),
        CHSV(213, 80, 100 - (100 / 15) * 11),
        CHSV(213, 80, 100 - (100 / 15) * 12),
        CHSV(213, 80, 100 - (100 / 15) * 13),
        CHSV(213, 80, 100 - (100 / 15) * 14),
        CHSV(213, 80, 0)
    );
}
