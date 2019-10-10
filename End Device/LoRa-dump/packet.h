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

    void debug(HardwareSerial *s) {
        s->print(status);
        s->print('/');
        s->print(latitude);
        s->print('/');
        s->print(longitude);
        s->print('/');
        s->print(temperature);
        s->print('/');
        s->print(humidity);
        s->print('/');
        s->print(battery);
        s->print('/');
        s->print(radiation);
        s->print('\r\n');
    }
};