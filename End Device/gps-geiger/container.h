#pragma once

#include "Arduino.h"

#include "Time.h"

struct Container {
    int32_t latitude = 0;
    int32_t longitude = 0;
    time_t time = 0;
    int32_t geiger = 0;
};
