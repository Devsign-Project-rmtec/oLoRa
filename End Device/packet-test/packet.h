#pragma once

#include "Arduino.h"

class Packet {
public:
    char latitude[11] = {0};
    char longitude[12] = {0};
    //uint16_t altitude = 0;
    int16_t temperature = -99;
    int16_t humidity = -99;
    int8_t battery = 0;
};