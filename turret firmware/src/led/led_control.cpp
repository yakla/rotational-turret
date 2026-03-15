#include "led_control.h"

#include <FastLED.h>

#include "shared/app_state.h"

[[noreturn]] void LedControl(void *pvParameters) {
    while (true) {
        static uint8_t currentIndex = 0;
        switch (currentMode) {
            case MANUAL_CONTROL:
                fill_palette(leds, constants::ledAmount, currentIndex++, 255 / constants::ledAmount,
                             constants::purplePalette, 255, LINEARBLEND);
                break;
            case FOLLOWER:
                fill_palette(leds, constants::ledAmount, currentIndex++, 255 / constants::ledAmount,
                             constants::bluePalette, 255, LINEARBLEND);
                break;
            case LOCK:
                fill_solid(leds, constants::ledAmount, CRGB::Red);
                break;
        }

        FastLED.show();
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
}
