#include <Arduino.h>

#include "MPU6050.h"
#include "DFRobot_SHT20.h"
#include <Wire.h>

#define LoRa Serial1

#include "packet.h"

Packet packet;

MPU6050 imu; // I2C 0x68
DFRobot_SHT20 sht20; // I2C 0x40

void send_packet(HardwareSerial *s);
char digit2HEXchar(byte d);

void setup() {
    Serial.begin(115200);
    LoRa.begin(38400);
    delay(1000);

    packet.status = 0;
    packet.latitude = (0UL << 31) | (35UL << 23) | (549411UL << 0);
    packet.longitude = (0UL << 31) | (127UL << 23) | (390688UL << 0);
    packet.temperature = 2521;
    packet.humidity = 5454;
    packet.battery = 75;
    packet.radiation = 75;
}

void loop() {
    // int16_t x, y, z;
    // imu.getAcceleration(&x, &y, &z);
    // Serial.print(x);
    // Serial.print('\t');
    // Serial.print(y);
    // Serial.print('\t');
    // Serial.print(z);
    // Serial.println();
    // delay(10);

    // float t, h;
    // Serial.print("reading Temperature...");
    // t = sht20.readTemperature();
    // Serial.println("OK");
    // h = sht20.readHumidity();
    // Serial.print(t);
    // Serial.print('\t');
    // Serial.print(h);
    // Serial.println();
    // delay(100);

    if(Serial.available()) LoRa.write(Serial.read());
    if(LoRa.available()) Serial.write(LoRa.read());

    send_packet(&LoRa);
    // Serial.println(UBRR1, BIN);
    // delay(1000);
}


void send_packet(HardwareSerial* s) {
    static long oldTime = 0;

    packet.radiation = random(10, 500);
    packet.battery = random(0, 100);

    //packet.debug(&Serial);

    long nowTime = millis();
    if(nowTime - oldTime > 10000){
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