#include <Arduino.h>
#include <IrSensor/IrSensor.h>


IrSensor::IrSensor(int _pinNum)
{
    pinNum = _pinNum;
    offset = 0;
}

int IrSensor::ReadNormal(){
    return analogRead(pinNum); 
}

int IrSensor::Read(){
    return (analogRead(pinNum) - offset); 
}

int IrSensor::GetPinNum(){
    return pinNum;
}


void IrSensor::SetOffset(int _offset)
{
    offset = _offset;
}

int IrSensor::GetOffset()
{
    return offset;
}