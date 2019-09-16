#pragma once

#include "Arduino.h"
#include <SoftwareSerial.h>
#include "packet.h"

class GPS_Processor {
private:
    SoftwareSerial* s;
    Packet* p;

public:
    GPS_Processor(SoftwareSerial* hwSerial, Packet* packet) : s(hwSerial), p(packet) {

    } 

    void task() {
        // sequence
        // 0: $ not found
        // 1: looking for GPRMC
        // 2: parsing
        static int8_t seq = 0;
        static int8_t waitLength = 1;
        static int8_t commas = 0;
        static char buffer[15];
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
                        case 3: // latitude string
                            buffer[idx++] = c;
                            break;

                        case 4: // latitude direction
                            latdir = (c == 'N');

                            strcpy(npart, strtok(buffer, "."));
                            strcpy(dpart, strtok(buffer, "."));
                            memset(buffer, 0, sizeof(buffer));

                            n = atoi(npart);
                            d = atoi(dpart);
                            integer = n / 100;
                            decimal = ((n % 100) * 1000000 + d)/60;
                            latitude = (latdir ? 0 : 1) << 31 | integer << 23 | (decimal & 0x7FFFFF);


                            idx = 0;
                            seq = 2; // inserted due to compiler bug
                            break;

                        case 5: // longitude string
                            buffer[idx++] = c;
                            break;

                        case 6: // longitude direction
                            londir = (c == 'E');

                            strcpy(npart, strtok(buffer, "."));
                            strcpy(dpart, strtok(buffer, "."));
                            memset(buffer, 0, sizeof(buffer));

                            n = atoi(npart);
                            d = atoi(dpart);
                            integer = n / 100;
                            decimal = ((n % 100) * 1000000 + d)/60;
                            longitude = (londir ? 0 : 1) << 31 | integer << 23 | (decimal & 0x7FFFFF);

                            idx = 0;
                            commas = 0;
                            seq = 0;

                            p->latitude = latitude;
                            p->longitude = longitude;

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