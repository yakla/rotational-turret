#pragma once

#include <Arduino.h>

bool setupCamera();
void ip_listener();
[[noreturn]] void udp_stream_task(void *pvParameters);
