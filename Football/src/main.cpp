#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Drive/Drive.h>
#include <Compass/Compass.h>
#include <IrSensor/IrSensor.h>
#include <Schuss/Schuss.h>
#include <MathJ/MathJ.h>
#include <Lichtschranke/Lichtschranke.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64 
#define OLED_RESET     4 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Drive drive = Drive();

Compass compass = Compass(4);

IrSensor ir[5] = {A8,A9,A10,A11,A12};

#define SchussPin 32
Schuss schuss(SchussPin);

MathJ math;

#define LichtschrankenPin A7
Lichtschranke lichtschranke(LichtschrankenPin);

#define LED 30

#define StatusLed 13

#define irsensoren 5

#define maxCount 5

#define IrResolution 100

#define LichtschrankePin A7

int compassValue;

int delayTime;

int count,maxValue[irsensoren],dir,irValue;

enum Phases
{
  Ballsuchen,Tor
};

Phases currentPhase = Ballsuchen;


void setup() {
  digitalWrite(StatusLed,LOW);
  //Werte zuweisen
  count = 0;
  delayTime = 0;
  for (size_t i = 0; i < irsensoren; i++)
  {
    maxValue[i] = 0;
  }
  
  dir = 0;

  //klassen bauen
  drive.Begin();

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
  {
    for(;;);
  }

  display.clearDisplay();

  schuss.Begin();
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
  digitalWrite(StatusLed,HIGH);
}





void loop() 
{
  unsigned long premil = millis();
  
  if(currentPhase == Ballsuchen)
  {
    noInterrupts();
    compassValue = compass.Read();
    interrupts();

    //compassValue = compass.Read();
  
    //ir sensoren
    if(count < maxCount)
    {
      for (size_t i = 0; i < irsensoren; i++)
      {
        int currentValue = ir[i].Read();

        if(currentValue > maxValue[i])
        {
          maxValue[i] = currentValue;
          //maxValues[i] = values[i];
        }
      }
    }
    else
    {
      int maxSensor = 0;
      for (size_t i = 0; i < irsensoren; i++)
      {
        if(maxValue[i] > maxSensor)
        {
          dir = i;
          maxSensor = maxValue[i];
        }
      }
      irValue = maxSensor * IrResolution;

      for (size_t i = 0; i < irsensoren; i++)
      {
        maxValue[i] = 0;
      }
      count = 0;
    }
    
    count ++;

    if(irValue >= 4500)
    {
      switch(dir)
      {
        case 0:
          drive.DriveLeft(170);
          break;

        case 1:
          drive.DriveLeft(120);
          break;

        case 2:
          drive.DriveForward(150);
          break;

        case 3:
          drive.DriveRight(120);
          break;

        case 4:
          drive.DriveRight(170);
          break;
      }
    }
    else
    {
      drive.Stop();
    }
    
    
  
    if(lichtschranke.Ball())
    {
      schuss.Schiese();
    }

    schuss.update();

    //Debugging
    display.setCursor(0,0);
    display.setTextSize(1);
    display.setTextColor(WHITE);

    display.println((String)dir + "  " + (String)irValue);
    display.println();
    display.println(millis() - premil);
    display.println();
    display.println(ir[2].Read() * IrResolution);
    display.display();
    display.clearDisplay(); 

    
  }
  else if(currentPhase == Tor)
  {
    //Tor finden
    //schiessen
    //.
    //.
    //.
  }
}





