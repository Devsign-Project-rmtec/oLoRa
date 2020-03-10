#pragma once

#include "Arduino.h"

#include "packet.h"

#define ADC_BAT_PIN A3
#define BAT_VOLTAGE_DIV_FACTOR 0.24

// Battery: Samsung ICR18650-26J 3.7V 2600mAh 
// https://lygte-info.dk/review/batteries2012/Samsung%20ICR18650-26F%202600mAh%20(Pink)%20UK.html

// 4.15V as full charge, 3.6V as full discharge (LDO consideration)
// 100% : 4.150V
//  90% : 4.025V
//  80% : 3.948V
//  70% : 3.886V
//  60% : 3.833V
//  50% : 3.783V
//  40% : 3.740V
//  30% : 3.723V
//  20% : 3.706V
//  10% : 3.670V
//   0% : 3.600V


class Battery {
private:
    //uint8_t stateReadPin;
    uint8_t voltageReadPin = ADC_BAT_PIN;
    const float vTable[11] = { 4.150, 4.025, 3.948, 3.886, 3.833, 3.783, 3.740, 3.723, 3.706, 3.670, 3.600 };
    const float vRef = 1.1;
    int adc = 0;
    float readVoltage = 0;
    byte level = 0;
    bool state = false;

public:
    Battery() {
        analogReference(INTERNAL);
        //pinMode(stateReadPin, 0);
        pinMode(voltageReadPin, 0);
    }

    void read() {
        adc = analogRead(voltageReadPin);
        //state = digitalRead(stateReadPin);
        // readVoltage = adc / 1024.0 * 1.1 / 0.24;
        readVoltage = adc * 4.5837 / 1024.0;
    }

    uint8_t getLevel() {
        uint8_t level = 100;
        for(int i = 0; i < 10; i++) {
            if(vTable[i + 1] > readVoltage) {
                level -= 10;
            }
        }
    }

    void task() {

    }
}