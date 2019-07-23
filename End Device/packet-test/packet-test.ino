#include <SoftwareSerial.h>

SoftwareSerial GPS(4, 5);

typedef struct {
    char latitude[12] = {0};
    char longitude[13] = {0};
    // uint16_t altitude = 0;
    // char temperature[4] = {0};
    // char humidity[3] = {0};
    // int8_t battery = 0;
} PACKET;

PACKET packet;

void setup(){
    Serial.begin(115200);
    GPS.begin(9600);
}

void loop(){
    gps_processor(&GPS);
    send_packet(&Serial);
    //gps_bridge();
}

void gps_bridge(){
    if(GPS.available()) Serial.write(GPS.read());
}

void gps_processor(SoftwareSerial *s) {
    // sequence
    // 0: $ not found
    // 1: looking for GPRMC
    // 2: parsing
    volatile static int seq = 0;
    static int waitLength = 1;
    volatile static int commas = 0;
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
                        strcpy(packet.latitude, buffer);
                        memset(buffer, 0, sizeof(buffer));
                        idx = 0;
                        seq = 2;
                        break;

                    case 5:
                        buffer[idx++] = c;
                        break;

                    case 6:
                        buffer[idx++] = c;
                        strcpy(packet.longitude, buffer);
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

void send_packet(HardwareSerial* s) {
    static long oldTime = 0;
    long nowTime = millis();
    if(nowTime - oldTime > 4000){
        s->print("at+send=lora:2:");
        for(int i = 0; i < sizeof(packet); i++) {
            byte v = reinterpret_cast<char *>(&packet)[i];
            s->print(digit2HEXchar(v >> 4 & 0x0F));
            s->print(digit2HEXchar(v & 0x0F));
        }
        s->print("\r\n");
        
        oldTime = nowTime;
    }
}

char digit2HEXchar(byte d){
    if(d < 10) return d + '0';
    else return d + 'A' - 10;
}