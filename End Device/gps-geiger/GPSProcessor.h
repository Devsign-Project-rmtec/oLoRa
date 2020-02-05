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
        bool active = false;
    } data;

    char *strsep(char **stringp, const char *delim) {
        char *start = *stringp;
        char *p;

        p = (start != NULL) ? strpbrk(start, delim) : NULL;

        if (p == NULL)
        {
            *stringp = NULL;
        }
        else
        {
            *p = '\0';
            *stringp = p + 1;
        }

        return start;
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

    bool isActive() {
        return data.active;
    }

    void task() {
        static char buffer[100];
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

        static bool availablity = false;
        
        static int8_t idx = 0;
        static char c;
        static char *p = NULL;
        static char *pbuffer = NULL;
        
        if (s->available()) {
            c = s->read();
            if(c != '$') buffer[idx++] = c;
            else {
                // $GPRMC,114455.532,A,3735.0079,N,12701.6446,E,0.000000,121.61,110706,,*0A 
                //       ^1         ^2^3        ^4^5         ^6^7       ^8     ^9   10^^11
                pbuffer = buffer;
                p = strsep(&pbuffer, ",");
                if(!strcmp("GPRMC", p)) { // 1st comma
                    p = strsep(&pbuffer, ","); // 2nd comma
                    datetime.hour   = (p[0] - '0') * 10 + (p[1] - '0');
                    datetime.minute = (p[2] - '0') * 10 + (p[3] - '0');
                    datetime.second = (p[4] - '0') * 10 + (p[5] - '0');

                    p = strsep(&pbuffer, ","); // 3rd comma
                    availablity = (p[0] == 'A');
                    data.active = availablity;

                    if(!availablity) {
                        strsep(&pbuffer, ",");
                        strsep(&pbuffer, ",");
                        strsep(&pbuffer, ",");
                        strsep(&pbuffer, ",");
                        strsep(&pbuffer, ",");
                        strsep(&pbuffer, ",");
                    }
                    else {
                        p = strsep(&pbuffer, "."); // dot
                        integer = atoi(p);
                        p = strsep(&pbuffer, ","); // 4th comma
                        decimal = atoi(p);
                        decimal = ((integer % 100) * 10000 + decimal) * 1.666667;
                        integer /= 100;

                        p = strsep(&pbuffer, ","); // 5th comma
                        latdir = (p[0] == 'N');
                        latitude = (latdir ? 1 : -1) * (integer * 1000000 + decimal);

                        p = strsep(&pbuffer, "."); // dot
                        integer = atoi(p);
                        p = strsep(&pbuffer, ","); // 6th comma
                        decimal = atoi(p);
                        decimal = ((integer % 100) * 10000 + decimal) * 1.666667;
                        integer /= 100;

                        p = strsep(&pbuffer, ","); // 7th comma
                        londir = (p[0] == 'E');
                        longitude = (londir ? 1 : -1) * (integer * 1000000 + decimal);

                        p = strsep(&pbuffer, ","); // 8th comma

                        p = strsep(&pbuffer, ","); // 9th comma
                    }

                    p = strsep(&pbuffer, ","); // 10th comma
                    // memset(debug_buf, 0, sizeof(debug_buf));
                    // memcpy(debug_buf, p, sizeof(debug_buf));
                    datetime.day   = (p[0] - '0') * 10 + (p[1] - '0');
                    datetime.month = (p[2] - '0') * 10 + (p[3] - '0');
                    datetime.year  = (p[4] - '0') * 10 + (p[5] - '0');

                    // set data container
                    setTime(datetime.hour, datetime.minute, datetime.second, datetime.day, datetime.month, datetime.year);
                    data.latitude = latitude;
                    data.longitude = longitude;
                    
                }
                memset(buffer, 0, sizeof(buffer));
                idx = 0;
            }
        }
        
    }
};
