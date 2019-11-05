#pragma once

#include "Arduino.h"

#include "Time.h"

struct Container {
    double latitude = 0;
    double longitude = 0;
    time_t time = 0;
    float geiger = 0;
}