#include <Schuss/Schuss.h>
#include <Arduino.h>

Schuss::Schuss(int pinNum)
{
    pin = pinNum;
    shot = false;
    premil = 0;
}

void Schuss::Schiese()
{
    if(!shot)
    {
        digitalWrite(pin,HIGH);
        premil = millis();
        shot = true;
    }
    
}

void Schuss::Begin()
{
    pinMode(pin,OUTPUT);
    digitalWrite(pin,LOW);
}

void Schuss::update()
{
    if(shot)
    {
        if(millis() - premil >= delayValue)
        {
            shot = false;
            digitalWrite(pin,LOW);
        }
    }
    
}