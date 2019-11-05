#include "MPU6050.h"
#include "DFRobot_SHT20.h"
#include <Wire.h>

#include "oLoRa-GPS-Geiger.h"

#define LoRa Serial1

MPU6050 imu; // I2C 0x68
DFRobot_SHT20 sht20; // I2C 0x40
oLoRa_GPS_Geiger cop;

void setup() {
    Serial.begin(115200);
    LoRa.begin(115200);
    Wire.begin();
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
    cop.updateData();
    Serial.println(cop.getLatitude());
    delay(1000);
}
