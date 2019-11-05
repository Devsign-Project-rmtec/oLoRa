#pragma once

#include "Arduino.h"

#include <Wire.h>

#include "Time.h"

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

public: 
    oLoRa_GPS_Geiger() {
        
    }

    void updateData() {
        struct {
            uint32_t lat;
            uint32_t lon;
            time_t t;
            uint16_t d;
        } packet;
        byte rcv[sizeof(packet)];

        Wire.beginTransmission(COPROCESSOR_ADDR);
        Wire.write(DATA_ALL);
        Wire.endTransmission();
        Wire.requestFrom(0x10, sizeof(packet));
        Wire.readBytes(rcv, sizeof(packet));
        Wire.endTransmission();

        memcpy(&packet, rcv, sizeof(packet));
        memcpy(&latitude_raw, &packet.lat, sizeof(uint32_t));
        memcpy(&longitude_raw, &packet.lon, sizeof(uint32_t));
        memcpy(&t, &packet.t, sizeof(time_t));
        memcpy(&geiger_raw, &packet.d, sizeof(uint16_t));
    }
    
    double getLatitude() {
        bool s = latitude_raw >> 31 & 0x80000000;
        uint8_t i = (latitude_raw & 0x7F800000) >> 23;
        uint32_t d = (longitude_raw & 0x007FFFFF);
        latitude = (s ? -1 : 1) * (i + (double)d / 1000000);
        return latitude;
    }

    double getLongitude() {
        bool s = longitude_raw >> 31 & 0x80000000;
        uint8_t i = (longitude_raw & 0x7F800000) >> 23;
        uint32_t d = (longitude_raw & 0x007FFFFF);
        longitude = (s ? -1 : 1) * (i + (double)d / 1000000);
        return longitude;
    }

    time_t getTime() {
        return t;
    }

};

