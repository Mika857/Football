#include <Lichtschranke/Lichtschranke.h>
#include <Arduino.h>

Lichtschranke::Lichtschranke(int pinNum)
{
    this->pinNum = pinNum;
}

void Lichtschranke::Begin()
{

}

bool Lichtschranke::Ball()
{
    return analogRead(pinNum) <= 600;
}