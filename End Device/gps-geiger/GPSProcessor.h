#pragma once

#include "Arduino.h"
#include <SoftwareSerial.h>
#include "packet.h"
#include "container.h"
#include "Time.h"

class GPS_Processor {
private:
    HardwareSerial* s;

    struct GPSdata{
        double latitude = 0;
        double longitude = 0;
    } data;

    uint32_t atoui32(const char* str) {
        uint32_t val = 0;
        for(int i = 0; i < strlen(str); i++) {
            val *= 10;
            val += str[i] - '0';
        }

        return val;
    }

public:
    GPS_Processor(HardwareSerial* hwSerial) : s(hwSerial) {
        setTime(0);
    } 

    double getLatitude() {
        return data.latitude;
    }

    double getLongitude() {
        return data.longitude;
    }

    struct GPSdata getDataSet() {
        return data;
    }

    void task() {
        // sequence
        // 0: $ not found
        // 1: looking for GPRMC
        // 2: parsing
        static volatile int8_t seq = 0;
        static int8_t waitLength = 1;
        static int8_t commas = 0;
        static char buffer[15];
        static char time_str[7] = { 0 };
        static struct DateTime {
            byte year;
            byte month;
            byte day;
            byte hour;
            byte minute;
            byte second;
        } time;
        static char npart[8] = { 0 };
        static char dpart[8] = { 0 };
        static int32_t n;
        static int32_t d;
        static uint32_t integer;
        static int32_t decimal;
        static int32_t latitude = 0;
        static int32_t longitude = 0;
        static bool latdir = false; // S:false, N:true
        static bool londir = false; // W:false, E:true
        
        static int8_t idx = 0;
        static char c;
        if (s->available() >= waitLength) {
            switch (seq) {
            case 0: // $ not found
                c = s->read();
                if (c == '$'){
                    seq = 1;
                    waitLength = 5;
                }
                break;

            case 1: // looking for GPRMC
                memset(buffer, 0, sizeof(buffer));
                for(idx = 0; idx < 5; idx++) buffer[idx] = s->read();
                if(!strcmp(buffer, "GPRMC")) {
                    waitLength = 1;
                    seq = 2;
                    idx = 0;
                    memset(buffer, 0, sizeof(buffer));
                }
                else seq = 0;
                break;

            case 2: // parsing
            // $GPRMC,114455.532,A,3735.0079,N,12701.6446,E,0.000000,121.61,110706,,*0A 
            //       ^1         ^2^3        ^4           ^5^6       ^7     ^8     ^^10   
                c = s->read();
                if(c == ',') {
                    commas++;
                }
                else {
                    switch(commas) {
                        case 1: // time_str time string
                            buffer[idx++] = c;
                            break;

                        case 2: 
                            strcpy(time_str, buffer);
                            memset(buffer, 0, sizeof(buffer));
                            idx = 0;

                            time.hour = (time_str[0] - '0') * 10 + (time_str[1] - '0');
                            time.minute = (time_str[2] - '0') * 10 + (time_str[3] - '0');
                            time.second = (time_str[4] - '0') * 10 + (time_str[5] - '0');

                            break;

                        case 3: // latitude string
                            buffer[idx++] = c;
                            break;

                        case 4: // latitude direction
                            latdir = (c == 'N');

                            //strcpy(npart, strtok(buffer, "."));
                            //strcpy(dpart, strtok(NULL, "."));
                            // memset(buffer, 0, sizeof(buffer));
                            idx = 0;

                            //n = atol(npart);
                            //d = atol(dpart);

                            //integer = n / 100;
                            //decimal = ((n % 100) * 100000 + d) * 0.1666667;
                            //latitude = (latdir ? 0 : 1) << 31 | integer << 23 | (decimal & 0x7FFFFF);
                            data.latitude = atof(String(buffer).c_str());
                            memset(buffer, 0, sizeof(buffer));

                            seq = 2; // inserted due to compiler bug
                            break;

                        case 5: // longitude string
                            buffer[idx++] = c;
                            break;

                        case 6: // longitude direction
                            londir = (c == 'E');

                            //strcpy(npart, strtok(buffer, "."));
                            //strcpy(dpart, strtok(NULL, "."));
                            //memset(buffer, 0, sizeof(buffer));
                            idx = 0;

                            //n = atol(npart);
                            //d = atol(dpart);
                            //integer = n / 100;
                            //decimal = ((n % 100) * 100000 + d) * 0.1666667;
                            //longitude = (londir ? 0 : 1) << 31 | integer << 23 | (decimal & 0x7FFFFF);
                            data.longitude = atof(String(buffer).c_str());
                            memset(buffer, 0, sizeof(buffer));
                            //data.latitude = latitude;
                            //data.longitude = longitude;


                            break;

                        case 9: // time_str date string
                            buffer[idx++] = c;
                            break;

                        case 10:
                            strcpy(time_str, buffer);
                            memset(buffer, 0, sizeof(buffer));
                            idx = 0;
                        
                            time.day = (time_str[0] - '0') * 10 + (time_str[1] - '0');
                            time.month = (time_str[2] - '0') * 10 + (time_str[3] - '0');
                            time.year = (time_str[4] - '0') * 10 + (time_str[5] - '0');

                            setTime(time.hour, time.minute, time.second, time.day, time.month, time.year);

                            // Serial.print(time.hour);
                            // Serial.print(':');
                            // Serial.print(time.minute);
                            // Serial.print(':');
                            // Serial.print(time.second);
                            // Serial.print(' ');
                            // Serial.print(time.year);
                            // Serial.print('/');
                            // Serial.print(time.month);
                            // Serial.print('/');
                            // Serial.print(time.day);
                            // Serial.println();

                            commas = 0;
                            seq = 0;
                            break;

                        default:
                            break;
                    } 
                } // end of if (c == ',') else
                break; // break of case 2:
            }
        }
    }
};