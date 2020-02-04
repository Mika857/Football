#include <Schuss/Schuss.h>
#include <Arduino.h>

Schuss::Schuss(int pinNum)
{
    pin = pinNum;
    pinMode(pin,OUTPUT);
    digitalWrite(pin,LOW);
}

void Schuss::Schiese()
{
    digitalWrite(pin,HIGH);
    delay(50);
    digitalWrite(pin,LOW);
}