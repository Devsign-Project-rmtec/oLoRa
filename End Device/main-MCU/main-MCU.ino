#include "pin_definition.h"

#include <Wire.h>
#include "MPU6050.h"
#include "DFRobot_SHT20.h"

#include "oLoRa-GPS-Geiger.h"

#define LoRa Serial1
#define TRANSMISSION_INTERVAL 4000

MPU6050 imu; // I2C 0x68
DFRobot_SHT20 sht20; // I2C 0x40
oLoRa_GPS_Geiger cop;

#include "container.h"
#include "packet.h"

Packet packet;

#include <SPI.h>
#include <SD.h>

File fd;


void setup() {
    cli();
    TCCR1A |= (1 << WGM11) | (1 << WGM10); // MODE14: Fast PWM, ICR1 as top
    TCCR1B |= (1 << WGM13) | (1 << WGM12);
    ICR1 = 7999; // TIMER1 will overflow on every 1ms
    sei();

    Serial.begin(115200);
    while(!Serial);
    LoRa.begin(115200);
    Wire.begin();
    SD.begin(PIN_SDSS);
    fd = SD.open(F("log.bin"), FILE_WRITE);
}

void loop() {
    cop.updateData();
    delay(500);
}

void isr_readTHI() {

}

void isr_readCoprocessor() {

}

void isr_writeSD() {
    fd.write((char *)&packet, 16);
}

void isr_sendLoRa() {

}

void isr_tasks() {
    static int cnt = TRANSMISSION_INTERVAL;
    if(--cnt < 0) {
        isr_readTHI();
        isr_readCoprocessor();
        isr_writeSD();
        isr_sendLoRa();
        cnt = TRANSMISSION_INTERVAL;
    }
}

ISR(TIMER1_ovf_vect) {
}

