#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Drive/Drive.h>
#include <Compass/Compass.h>
#include <IrSensor/IrSensor.h>
#include <Schuss/Schuss.h>
#include <MathJ/MathJ.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64 
#define OLED_RESET     4 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Drive drive = Drive();

Compass compass = Compass(34);

IrSensor ir[5] = {A8,A9,A10,A11,A12};

#define SchussPin 32
Schuss schuss(SchussPin);

MathJ math;

#define LED 22

#define irsensoren 5

#define maxCount 5

int compassValue;

int delayTime;

int count,maxValue,dir;

void setup() {
  //Werte zuweisen
  count = 0;
  delayTime = 0;
  maxValue = 0;
  dir = 0;

  //klassen bauen
  drive.Begin();

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
  {
    for(;;);
  }

  display.clearDisplay();


  //offset berechnen
  for (int j = 0; j < irsensoren; j++)
  {
    unsigned long premil = millis();
    int i = 0;
    long value = 0;

    while(millis() - premil < 1000)
    {
      value += ir[j].ReadNormal();
      i++;
    }

    value /= i;
    ir[j].SetOffset(value);
    value = 0;
    i = 0;
  }

  
  
  pinMode(SchussPin,OUTPUT);
  digitalWrite(SchussPin,LOW);
  //Status-Led anschalten
  pinMode(LED,OUTPUT);
  digitalWrite(LED,HIGH);
}





void loop() 
{
  unsigned long premil = millis();
  //anfangs drehung;
  compassValue = 0;

  int motorSpeed = map(abs(compassValue),0,180,32,210);
  

  if(compassValue < 0)
  {
    drive.RotateClockwise(motorSpeed);
  }
  else if(compassValue > 0)
  {
    drive.RotateCounterClockwise(motorSpeed);
  }
  else
  {
    drive.Stop();
  }
  
  //ir sensoren
  if(count < maxCount)
  {
    for (size_t i = 0; i < irsensoren; i++)
    {
      int currentValue = ir[i].Read();

      if(currentValue > maxValue)
      {
        maxValue = currentValue;
        dir = i; 
        //maxValues[i] = values[i];
      }
    }
  }
  else
  {
    count = 0;
    maxValue = ir[dir].Read();
  }
  
  count ++;

  
  if(dir == 2 && maxValue >= 600)
  {
    schuss.Schiese();
  }
 

  //Debugging
  display.setCursor(0,0);
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.println((String)dir + "  " + (String)maxValue);
  display.println();
  display.println(millis() - premil);
  display.display();
  display.clearDisplay(); 
}



