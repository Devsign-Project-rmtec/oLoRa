#pragma once

#include "Arduino.h"

#include "Time.h"

#define STATUS_GPS 0

struct Container {
    uint8_t status = 0;
    int32_t latitude = 0;
    int32_t longitude = 0;
    time_t time = 0;
    int16_t geiger = 0;
};

