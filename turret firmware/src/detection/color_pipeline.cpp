#include "color_pipeline.h"

#include <Arduino.h>
#include <esp_camera.h>

#include "hsv_utils.h"
#include "shared/app_state.h"


constexpr uint16_t red565 = 0xF800;

void drawCross(const camera_fb_t *fb, int cx, int cy, uint16_t color);

void moveToTarget(const int targetX, const int targetY, const camera_fb_t *fb);

void colorPipeLineDetection(camera_fb_t *fb) {
    if (!fb) return;

    if (fb->format != PIXFORMAT_RGB565) {
        esp_camera_fb_return(fb);
        return;
    }

    const uint8_t *buf = fb->buf;
    const size_t bufferSize = fb->len;

    int32_t sumDetectedPixelsX = 0;
    int32_t sumDetectedPixelsY = 0;

    uint16_t counterDetectedPixels = 0;

    for (size_t i = 0; i + 1 < bufferSize; i += 2) {
        const uint16_t pixel = (static_cast<uint16_t>(buf[i]) << 8) | buf[i + 1];
        const HsvPixel hsv = rgb565ToHsv(pixel);
        if (hsvMatchesRange(hsv, constants::dog_min_hsv, constants::dog_max_hsv)) {
            sumDetectedPixelsX += (i / 2) % fb->width;
            sumDetectedPixelsY += (i / 2) / fb->width;
            counterDetectedPixels++;
        }
    }


    const int32_t average_x = sumDetectedPixelsX / counterDetectedPixels;
    const int32_t average_y = sumDetectedPixelsY / counterDetectedPixels;

    drawCross(fb, average_x, average_y, red565);
    moveToTarget(average_x, average_y, fb);
    esp_camera_fb_return(fb);
}


static void setPixelRgb565(const camera_fb_t *fb, const int x, const int y, const uint16_t color) {
    if (x < 0 || y < 0 || x >= fb->width || y >= fb->height) return;

    uint8_t *buf = fb->buf;
    const size_t idx = (y * fb->width + x) * 2;
    buf[idx] = static_cast<uint8_t>(color >> 8);
    buf[idx + 1] = static_cast<uint8_t>(color & 0xFF);
}

void drawCross(const camera_fb_t *fb, const int cx, const int cy, const uint16_t color) {
    for (int dx = -3; dx <= 3; dx++) {
        setPixelRgb565(fb, cx + dx, cy, color);
    }
    for (int dy = -3; dy <= 3; dy++) {
        setPixelRgb565(fb, cx, cy + dy, color);
    }
}

void moveToTarget(const int targetX, const int targetY, const camera_fb_t *fb) {
    //need the hardware to know what the pulse need to be to move to the direction
    //going to use pid and the error is the angle interpolated by the target position from center and the fov of the camera.
    const int centerX = fb->width / 2;
    const int centerY = fb->height / 2;
    const int errorX = centerX - targetX;
    const int errorY = centerY - targetY;
    int error = std::sqrt(errorX * errorX + errorY * errorY);
    int horizontal_angle_error = (errorX / centerX) * (constants::horizontalFov / 2);
    int vertical_angle_error = (errorY / centerY) * (constants::verticalFov / 2);
    //TODO add it when the hardware is here !!
}
