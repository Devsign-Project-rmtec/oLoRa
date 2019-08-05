#pragma once

#include "Arduino.h"
#include "packet.h"

class GPS_Processor {
private:
    HardwareSerial* s;
    Packet* p;

public:
    GPS_Processor(HardwareSerial* hwSerial, Packet* packet) : s(hwSerial), p(packet) {

    } 

    void task() {
        // sequence
        // 0: $ not found
        // 1: looking for GPRMC
        // 2: parsing
        static int seq = 0;
        static int waitLength = 1;
        static int commas = 0;
        static char buffer[15];
        static int8_t idx = 0;
        static char c;
        static double flat = 0;
        static double flon = 0;
        static bool latdir = false; // N:false, S:true
        static bool londir = false; // W:false, E:true

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
                if(buffer == "GPRMC") {
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
                            // buffer[idx++] = c;
                            // memcpy((p + sizeof(p->latitude) - strlen(buffer))->latitude, buffer, strlen(buffer));
                            // memset(buffer, 0, sizeof(buffer));
                            latdir = (c == 'S');
                            flat = atof(buffer) * 0.01;
                            flat = (unsigned byte)flat + (flat - (unsigned byte)flat) / 60;

                            
                            memset(buffer, 0, sizeof(buffer));

                            idx = 0;
                            seq = 2; // inserted due to compiler bug
                            break;

                        case 5: // longitude string
                            buffer[idx++] = c;
                            break;

                        case 6: // longitude direction
                            // buffer[idx++] = c;
                            // memcpy((p + sizeof(p->longitude) - strlen(buffer))->longitude, buffer, strlen(buffer));
                            // memset(buffer, 0, sizeof(buffer));

                            londir = (c == 'E');
                            flon = atof(buffer) * 0.01;
                            flon = (unsigned byte)flon + (flon - (unsigned byte)flon) / 60;

                            memset(buffer, 0, sizeof(buffer));

                            idx = 0;
                            commas = 0;
                            seq = 0;

                            p->direction |= latdir << 0 | londir << 1;
                            p->latitude = flat;
                            p->longitude = flon;

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