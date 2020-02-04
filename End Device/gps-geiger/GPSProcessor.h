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
        int32_t latitude = 0;
        int32_t longitude = 0;
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

    int32_t getLatitude() {
        return data.latitude;
    }

    int32_t getLongitude() {
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
        static struct DateTime {
            int8_t year;
            int8_t month;
            int8_t day;
            int8_t hour;
            int8_t minute;
            int8_t second;
        } datetime;
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
                    commas = 0;
                    memset(buffer, 0, sizeof(buffer));
                }
                else seq = 0;
                break;

            case 2: // parsing
            // $GPRMC,114455.532,A,3735.0079,N,12701.6446,E,0.000000,121.61,110706,,*0A 
            //       ^1         ^2^3        ^4^5         ^6^7       ^8     ^9    10^^11   
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
                            //if(c != 'A') commas = 15;
                            
                            datetime.hour = (buffer[0] - '0') * 10 + (buffer[1] - '0');
                            datetime.minute = (buffer[2] - '0') * 10 + (buffer[3] - '0');
                            datetime.second = (buffer[4] - '0') * 10 + (buffer[5] - '0');
                            //datetime.hour = 11;
                            //datetime.minute = 22;
                            //datetime.second = 33;
                            memset(buffer, 0, sizeof(buffer));
                            idx = 0;

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
                            data.latitude = atof(String(buffer).c_str()) * 10000;
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

                            //n = atol(npart);
                            //d = atol(dpart);
                            //integer = n / 100;
                            //decimal = ((n % 100) * 100000 + d) * 0.1666667;
                            //longitude = (londir ? 0 : 1) << 31 | integer << 23 | (decimal & 0x7FFFFF);
                            data.longitude = atof(String(buffer).c_str()) * 10000;
                            memset(buffer, 0, sizeof(buffer));
                            idx = 0;
                            //data.latitude = latitude;
                            //data.longitude = longitude;


                            break;

                        case 9: // time_str date string
                            buffer[idx++] = c;
                            break;

                        case 10:
                            datetime.day = (buffer[0] - '0') * 10 + (buffer[1] - '0');
                            datetime.month = (buffer[2] - '0') * 10 + (buffer[3] - '0');
                            datetime.year = (buffer[4] - '0') * 10 + (buffer[5] - '0');
                            memcpy(debug_buf, buffer, sizeof(buffer));
                            memset(buffer, 0, sizeof(buffer));
                            idx = 0;
                            // datetime.day = 1;
                            // datetime.month = 2;
                            // datetime.year = 20;

                            
                            setTime(datetime.hour, datetime.minute, datetime.second, datetime.day, datetime.month, datetime.year);

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
