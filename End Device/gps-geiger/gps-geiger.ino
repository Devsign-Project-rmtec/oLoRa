#define GPS_SWSERIAL 1

#include <Wire.h>
#include "pin_definition.h"

#if GPS_SWSERIAL
#include <SoftwareSerial.h>
SoftwareSerial GPS(4, 5);
#else
#define GPS Serial
#endif

#define I2C_ADDR 0x10

class GeigerProcessor {
private:
    const float factor_usv = 0.0057;

public:
    Geiger() {
        analogReference(INTERNAL);
    }


};

void setup(){

    // 4khz pwm
    // timer1 prescaler8 icr249
    cli();
    TTCR1A = 1 << WGM10 | 1 << WGM11;
    TTCR1B |= 1 << WGM12 | 1 << WGM13 | 1 << CS11; // set prescaler to 8
    TIMSK1 |= 1 << OCIE1B; // use pin 10 as pwm output
    ICR1 = 249; // set top to 249
    sei();

    GPS.begin(9600);
    Wire.begin(I2C_ADDR);

    attachInterrupt(digitalPinToInterrupt(GEIGER_INPUT), geigerImpulse, FALLING);
}

void loop(){

}

void geigerImpulse(){

}

ISR(TIMER1_OVF_vect){
  
}

// run every 5ms
void HVcontroller(){
   previousMillis_hv = currentMillis;
   
   val = analogRead(HV_INPUT);     // read the input pin
   if (val<400) {
     analogWrite(10, 55+pwm_step);      //correct tube voltage
   }
   else {
     analogWrite(10, 29);      //correct tube voltage
   }
}