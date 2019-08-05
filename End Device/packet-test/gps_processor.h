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
        static String buffer;
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
                buffer = "";
                for(int i = 0; i < 5; i++) buffer += s->read();
                if(buffer == "GPRMC") {
                    waitLength = 1;
                    seq = 2;
                    idx = 0;
                    buffer = "";
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
                            // buffer[idx++] = c;
                            buffer += c;
                            break;

                        case 4: // latitude direction
                            // buffer[idx++] = c;
                            // memcpy((p + sizeof(p->latitude) - strlen(buffer))->latitude, buffer, strlen(buffer));
                            // memset(buffer, 0, sizeof(buffer));
                            latdir = (c == 'S');
                            flat = std::stof(buffer);
                            flat = flat/100 + (flat%100)*0.01/60;

                            
                            buffer = "";

                            idx = 0;
                            seq = 2; // inserted due to compiler bug
                            break;

                        case 5: // longitude string
                            // buffer[idx++] = c;
                            buffer += c;
                            break;

                        case 6: // longitude direction
                            // buffer[idx++] = c;
                            // memcpy((p + sizeof(p->longitude) - strlen(buffer))->longitude, buffer, strlen(buffer));
                            // memset(buffer, 0, sizeof(buffer));

                            londir = (c == 'E');
                            flon = std::stof(buffer);
                            flon = flon/100 + (flon%100)*0.01/60;

                            buffer = "";

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