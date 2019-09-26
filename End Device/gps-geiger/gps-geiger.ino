#define GPS_SWSERIAL 1

#include "Time.h"

#include <Wire.h>
byte i2c_command = 0;
enum {
    DATA_GPS = 1,
    DATA_TIME
};

#include "pin_definition.h"

#if GPS_SWSERIAL
#include <SoftwareSerial.h>
SoftwareSerial GPS(4, 5);
#else
#define GPS Serial
#endif

#define I2C_ADDR 0x10

#include "GeigerProcessor.h"
GeigerProcessor geiger;

#include "GPSProcessor.h"
GPS_Processor gps(&GPS);

void setup(){
    Serial.begin(115200);
    Serial.println("init");
    analogReference(INTERNAL);

    // 4khz pwm
    // timer1 prescaler8 icr249
    cli();
    TCCR1A = 1 << COM1A1 | 0 << COM1A0 | 1 << WGM11 | 0 << WGM10; // Mode 14: Fast PWM / TOP: ICR1 / Update of OCR1x at BOTTOM
    TCCR1B = 1 << WGM13 | 1 << WGM12 | 1 << CS10; 
    TIMSK1 |= 1 << TOIE1; // use pin 10 as pwm output
    ICR1 = 1999;
    sei();

    GPS.begin(9600);
    Wire.begin(I2C_ADDR);
    pinMode(13, 1);
    pinMode(9, 1);

    attachInterrupt(digitalPinToInterrupt(GEIGER_INPUT), interrupt0, FALLING);
    OCR1A = 0;
    Wire.onReceive(I2C_Receive);
    Wire.onRequest(I2C_Request);

}

void loop(){
    geiger.task();
    gps.task();
}

void interrupt0() {
    geiger.impulse();
}

void I2C_Receive(int nBytes) {
    i2c_command = Wire.read();
}

void I2C_Request() {
    static uint8_t len;
    static byte *p;
    switch(i2c_command) {
        case DATA_GPS:
            uint32_t lat = gps.getLatitude();
            uint32_t lon = gps.getLongitude();
            p = (byte *)malloc(sizeof(lat) + sizeof(lon));
            memcpy(p, &lat, sizeof(lat));
            memcpy(p + sizeof(lat), &lon, sizeof(lon));

            Wire.write(p, sizeof(lat) + sizeof(lon));
            break;

        case DATA_TIME:
            time_t t = now();
            p = reinterpret_cast<byte *>(&t);
            Wire.write(p, sizeof(time_t));
            break;
    }
}


ISR(TIMER1_OVF_vect){
    t1ovf();
}

void t1ovf(){
    // 400V -> 375
    // 226, 430+a
    int val = analogRead(HV_INPUT);
    if(val < 375) {
        OCR1A = 1500;
    }
    else {
        OCR1A = 150;
    }
}

// // run every 5ms
// void HVcontroller(){
//    previousMillis_hv = currentMillis;
   
//    val = analogRead(HV_INPUT);     // read the input pin
//    if (val<400) {
//      analogWrite(10, 55+pwm_step);      //correct tube voltage
//    }
//    else {
//      analogWrite(10, 29);      //correct tube voltage
//    }
// }