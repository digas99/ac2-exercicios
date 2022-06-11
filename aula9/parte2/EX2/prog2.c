#include <detpic32.h>

void setPWM(unsigned int );

void setPWM(unsigned int dutyCycle) {
    if (dutyCycle >= 0 && dutyCycle <= 100)
        OC1RS = (PR3+1)*dutyCycle/100;
}