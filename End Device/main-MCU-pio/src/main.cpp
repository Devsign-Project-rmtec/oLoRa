#include <Arduino.h>

#include "MPU6050.h"
#include "DFRobot_SHT20.h"
#include <Wire.h>

#define LoRa Serial1

MPU6050 imu; // I2C 0x68
DFRobot_SHT20 sht20; // I2C 0x40

void setup() {
    Serial.begin(115200);
    LoRa.begin(38400);
    delay(1000);
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
    // Serial.println(UBRR1, BIN);
    // delay(1000);
}
