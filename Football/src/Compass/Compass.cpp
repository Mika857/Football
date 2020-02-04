#include <Compass/Compass.h>
#include <Arduino.h>

Compass::Compass(int _pinNum)
{
    pinNum = _pinNum;
    pinMode(pinNum,INPUT);
    startValue = pulseIn(pinNum,HIGH)/100;

}

int Compass::GetValue()
{
    noInterrupts();
    int prevalue = pulseIn(pinNum,HIGH)/100;

    prevalue -= startValue;

    if(prevalue < 0)
    {
        prevalue = 360 - abs(prevalue);
    }

    if(prevalue > 180)
    {
        prevalue -= 360;
    }
    interrupts();
    return prevalue;
}