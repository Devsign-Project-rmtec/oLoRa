#include "gps_processor.h"
#include "HTU21D.h"
#include "packet.h"
#include <SoftwareSerial.h>

//#define GPS Serial1
#define LoRa Serial

SoftwareSerial GPS(4, 5);

Packet packet;
GPS_Processor gps_processor(&GPS, &packet);
HTU21D htu(HTU21D_RES_RH12_TEMP14);


void setup(){
    LoRa.begin(115200);
    GPS.begin(9600);
    htu.begin();
}

void loop(){
    gps_processor.task();
    htu.task(&packet);
    send_packet(&Serial);
    //gps_bridge();
}

void gps_bridge(){
    if(GPS.available()) Serial.write(GPS.read());
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