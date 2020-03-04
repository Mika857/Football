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
#include <Ultrasonic.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64 
#define OLED_RESET     4 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Drive drive = Drive();

Compass compass = Compass(4);

#define irsensoren 8

IrSensor ir[irsensoren] = {A8,A9,A10,A11,A12,A13,A14,A15};

#define SchussPin 32
Schuss schuss(SchussPin);

MathJ math;

#define LichtschrankenPin A7
Lichtschranke lichtschranke(LichtschrankenPin);

//Ultrasonic
#define Ultraschallsensoren 4
Ultrasonic uRechts(12,13);
Ultrasonic uLinks(10,11);
Ultrasonic uHinten(8,9);
Ultrasonic uVorne();

#define LED 30

#define StatusLed 13



#define maxCount 5

#define IrResolution 1

#define LichtschrankePin A7

#define Speed 100

int compassValue;

int delayTime;

int count,maxValue[irsensoren],dir,irValue;

enum Phases
{
  Ballsuchen,Tor,Debugging
};

Phases currentPhase = Debugging;


void setup() {
  Serial.begin(9600);
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
  display.setCursor(0,0);
  display.setTextSize(1);
  display.setTextColor(WHITE);

  schuss.Begin();
  //offset berechnen
  for (int j = 0; j < irsensoren; j++)
  {
    int i = 0;
    long value = 0;

    while(i < 200)
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


          //werte anpassen falls nicht ganz richtig
          if(i == 2)
          {
            maxValue[i] += 100;
          }
          if(i == 4)
          {
            maxValue[i] -= 30;
          }
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

    if(irValue >= 0)
    {
      switch(dir)
      {
        case 0:
          drive.DriveLeft(Speed * 1.2);
          break;

        case 1:
          drive.DriveLeft(Speed);
          break;

        case 2:
          drive.DriveForward(Speed * 1.3);
          break;

        case 3:
          drive.DriveRight(Speed);
          break;

        case 4:
          drive.DriveRight(Speed * 1.2);
          break;
        case 5:
          drive.DriveBackward(Speed * 1.3);
          break;

        case 6:
          drive.DriveDiagonalRechtsHinten(Speed * 1.3);
          break;

        case 7:
          drive.DriveBackward(Speed * 1.3);
          break;
      }
    }
    else
    {
      drive.Stop();
    }
    
    
  
    while(lichtschranke.Ball())
    {
      drive.Stop();
    }

    schuss.update();
    

    //Debugging

    display.println((String)dir + ": " + (String)irValue) + " str";
  }
  else if(currentPhase == Tor)
  {
    //Tor finden
    //schiessen
    //.
    //.
    //.
  }
  else if(currentPhase == Debugging)
  {
    
    display.println(uHinten.read());
    display.println(uLinks.read());
    display.println(uRechts.read()); 
  }

  display.println((String)(millis() - premil) + " ms/l");
  display.display();
  display.clearDisplay(); 
  display.setCursor(0,0);
}





