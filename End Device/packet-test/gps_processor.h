#pragma once

#include "Arduino.h"
#include "packet.h"

class GPS_Processor {
private:
    SoftwareSerial* s;
    Packet* p;

public:
    GPS_Processor(SoftwareSerial* swSerial, Packet* packet) : s(swSerial), p(packet) {

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
                for(idx = 0; idx <5; idx++) buffer[idx] = s->read();
                if(!strcmp(buffer, "GPRMC")) {
                    waitLength = 1;
                    seq = 2;
                    idx = 0;
                    memset(buffer, 0, sizeof(buffer));
                }
                else seq = 0;
                break;

            case 2: // parsing
                c = s->read();
                if(c == ',') {
                    commas++;
                }
                else {
                    switch(commas) {
                        case 3:
                            buffer[idx++] = c;
                            break;

                        case 4:
                            buffer[idx++] = c;
                            memcpy((p + sizeof(p->latitude) - strlen(buffer))->latitude, buffer, strlen(buffer));
                            memset(buffer, 0, sizeof(buffer));
                            idx = 0;
                            seq = 2;
                            break;

                        case 5:
                            buffer[idx++] = c;
                            break;

                        case 6:
                            buffer[idx++] = c;
                            memcpy((p + sizeof(p->longitude) - strlen(buffer))->longitude, buffer, strlen(buffer));
                            memset(buffer, 0, sizeof(buffer));
                            idx = 0;
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