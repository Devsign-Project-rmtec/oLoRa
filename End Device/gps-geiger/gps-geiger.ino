#define GPS_SWSERIAL 0

#include "Time.h"

#include <Wire.h>
#define DATA_GPS 0x01
#define DATA_TIME 0x02
#define DATA_USV 0x03
#define DATA_ALL 0x04
volatile byte i2c_command = 0;

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
    Serial.begin(9600);
    Serial.println("init");
    analogReference(INTERNAL);

    // 4khz pwm
    cli();
    TCCR1A = 1 << COM1B1 | 0 << COM1B0 | 1 << WGM11 | 0 << WGM10; // Mode 14: Fast PWM / TOP: ICR1 / Update of OCR1x at BOTTOM
    TCCR1B = 1 << WGM13 | 1 << WGM12 | 1 << CS10; 
    TIMSK1 |= 1 << TOIE1;
    ICR1 = 1999;
    sei();

    GPS.begin(9600);
    Wire.begin(I2C_ADDR);
    pinMode(13, 1);
    pinMode(HV_CTRL, 1);

    attachInterrupt(digitalPinToInterrupt(GEIGER_INPUT), interrupt0, FALLING);
    OCR1B = 0;
    Wire.onReceive(I2C_Receive);
    Wire.onRequest(I2C_Request);
}

void loop(){
    geiger.task();
    gps.task();
    //if(GPS.available()) Serial.write(GPS.read());
}

void interrupt0() {
    geiger.impulse();
}

void I2C_Receive(int nBytes) {
    i2c_command = Wire.read();
    Serial.println(i2c_command);
}

void I2C_Request() {
    static uint8_t len;
    static byte *p;
    if (i2c_command == DATA_GPS) {
        uint32_t lat = gps.getLatitude();
        uint32_t lon = gps.getLongitude();
        p = (byte *)malloc(sizeof(lat) + sizeof(lon));
        memcpy(p, &lat, sizeof(lat));
        memcpy(p + sizeof(lat), &lon, sizeof(lon));

        Wire.write(p, sizeof(lat) + sizeof(lon));
    }
    else if (i2c_command == DATA_TIME) {
        time_t t = now();
        p = reinterpret_cast<byte *>(&t);
        Wire.write(p, sizeof(time_t));
    }
    else if (i2c_command == DATA_USV) {
        uint16_t d = (uint16_t)(geiger.getUSV() * 100);
        Serial.println(d);
        p = reinterpret_cast<byte *>(&d);
        Wire.write(p, sizeof(uint16_t));
    }
    else if (i2c_command == DATA_ALL) {
        struct {
            uint32_t lat = gps.getLatitude();
            uint32_t lon = gps.getLongitude();
            time_t t = now();
            uint16_t d = (uint16_t)(geiger.getUSV() * 100);
        } packet;
        p = reinterpret_cast<byte *>(&packet);
        Wire.write(p, sizeof(packet));
    }
}

ISR(TIMER1_OVF_vect){
    geiger.adjHV();
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