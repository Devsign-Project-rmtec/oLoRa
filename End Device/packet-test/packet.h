#pragma once

#include "Arduino.h"

class Packet {
public:
    byte status = 0;
    int32_t latitude = 0;
    int32_t longitude = 0;
    int16_t temperature = -99;
    int16_t humidity = -99;
    int8_t battery = 0;
    int16_t radiation = 0;
};