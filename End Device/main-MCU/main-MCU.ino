#include "MPU6050.h"
#include "DFRobot_SHT20.h"
#include <Wire.h>

#define LoRa Serial1

MPU6050 imu; // I2C 0x68
DFRobot_SHT20 sht20; // I2C 0x40

void setup() {
    Serial.begin(115200);
    LoRa.begin(115200);
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
    float t, h;
    t = sht20.readTemperature();
    h = sht20.readHumidity();
    Serial.print(t);
    Serial.print('\t');
    Serial.print(h);
    Serial.println();
    delay(100);
}
