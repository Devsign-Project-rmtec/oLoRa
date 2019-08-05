#pragma once

#include "Arduino.h"

class Packet {
public:
    byte direction = 0;
    double latitude = 0;
    double longitude = 0;
    int16_t temperature = -99;
    int16_t humidity = -99;
    int8_t battery = 0;
};