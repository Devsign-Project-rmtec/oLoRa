#include "packet.h"

#define LoRa Serial1

Packet packet;

void setup() {
    Serial.begin(115200);
    LoRa.begin(115200);

    packet.status = 0;
    packet.latitude = (0UL << 31) | (35UL << 23) | (540121UL << 0);
    packet.longitude = (0UL << 31) | (127UL << 23) | (340121UL << 0);
    packet.temperature = 2703;
    packet.humidity = 2412;
    packet.battery = 50;
    packet.radiation = 25;

    randomSeed(analogRead(0));
}

void loop() {
    send_packet(&LoRa);
}

void send_packet(HardwareSerial* s) {
    static long oldTime = 0;

    packet.radiation = random(10, 500);
    packet.battery = random(0, 100);

    packet.debug(&Serial);

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