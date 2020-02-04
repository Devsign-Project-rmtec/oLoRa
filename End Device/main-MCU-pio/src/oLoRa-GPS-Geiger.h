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
#define DEBUG 0xFF


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
        uint8_t buffer[16] = { 0 };
        //Serial.println("begin");
        Wire.beginTransmission(COPROCESSOR_ADDR);
        //Serial.println("write");
        Wire.write(DATA_ALL);
        //Serial.println("end");
        Wire.endTransmission();
        //Serial.println("req");
        Wire.requestFrom(COPROCESSOR_ADDR, 14);
        delay(10);
        //if(Wire.available() != 32) return;
        //Serial.println("read");
        Wire.readBytes(buffer, 14);
        for(int i = 0; i < 14; i ++) {
            Serial.print(buffer[i], DEC);
            Serial.print(' ');
        }
        Serial.println();

        memcpy(&c, buffer, sizeof(c));
        Serial.print(c.latitude * 0.00001);
        Serial.print('\t');
        Serial.print(c.longitude * 0.00001);
        Serial.print('\t');
        Serial.print(year(c.time));
        Serial.print(' ');
        Serial.print(month(c.time));
        Serial.print(' ');
        Serial.print(day(c.time));
        Serial.print(' ');
        Serial.print(hour(c.time));
        Serial.print(' ');
        Serial.print(minute(c.time));
        Serial.print(' ');
        Serial.print(second(c.time));
        Serial.print(' ');
        Serial.print('\t');
        Serial.print(c.geiger * 0.01);
        Serial.print('\n');
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