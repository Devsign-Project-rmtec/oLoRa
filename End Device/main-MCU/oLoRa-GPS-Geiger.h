#pragma once

#include "Arduino.h"

#include <Wire.h>

#include "Time.h"

#include "container.h"

#define COPROCESSOR_ADDR 0x10
#define DATA_GPS 0x01
#define DATA_TIME 0x02
#define DATA_USV 0x03
#define DATA_ALL 0x04


class oLoRa_GPS_Geiger {
private:
    uint32_t latitude_raw = 0;
    uint32_t longitude_raw = 0;
    uint16_t geiger_raw = 0;
    time_t t = 0;
    double latitude = 0;
    double longitude = 0;
    float geiger = 0;

    Container c;

public: 
    oLoRa_GPS_Geiger() {
        
    }

    void updateData() {
        Wire.beginTransmission(COPROCESSOR_ADDR);
        Wire.write(DATA_GPS);
        Wire.endTransmission();
        Wire.requestFrom(COPROCESSOR_ADDR, 24);
        while(Wire.available() < 24);
        // Wire.readBytes((uint8_t *)&c, 24);
        while(Wire.available()) {
            Serial.print(Wire.read(), HEX);
            Serial.print(' ');
        }
        Serial.println();
    }
    
    double getLatitude() {
        return c.latitude;
    }

    double getLongitude() {
        return c.longitude;
    }

    time_t getTime() {
        return c.time;
    }

    float getUSV() {
        return c.geiger;
    }

    

};