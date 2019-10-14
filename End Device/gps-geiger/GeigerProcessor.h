#include "Arduino.h"


class GeigerProcessor {
private:
    const float factor_usv = 0.0057;
    uint16_t countCPS = 0;
    uint16_t cps = 0;
    uint16_t countCPM = 0;
    float cpm = 0;
    uint16_t countArray[6] = { 0 };
    int8_t countArrayIdx = 0;

public:
    Geiger() {
        analogReference(INTERNAL);
    }

    void impulse() {
        countCPS++;
        countCPM++;
    }

    void task() {
        static unsigned long task1Time = 0;
        static unsigned long task2Time = 0;
        unsigned long nowTime = millis();
        if(nowTime - task1Time > 1000) {
            cps = countCPS;
            countCPS = 0;
            countArray[countArrayIdx] += cps;
            //Serial.print("CPS: ");
            //Serial.println(cps);

            task1Time = nowTime;
        }
        if(nowTime - task2Time > 10000) {
            cpm = 0;
            for(int i = 0; i < 6; i++) {
                cpm += countArray[i];
                Serial.print(countArray[i]);
                Serial.print('\t');
            }
            //Serial.print(getCPM());
            //Serial.print('\t');
            //Serial.println(getUSV());

            if(++countArrayIdx == 6) countArrayIdx = 0;
            countArray[countArrayIdx] = 0;
            task2Time = nowTime;
        }
    }

    void adjHV() {
        int val = analogRead(HV_INPUT);
        if (val < 375) {
            OCR1B = 1500;
        } else {
            OCR1B = 150;
        }
    }

    uint16_t getCPS() { return cps; }
    uint16_t getCPM() { return cpm; }
    float getUSV() { return cpm * factor_usv; }

};
