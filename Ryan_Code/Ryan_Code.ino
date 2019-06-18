#include "Defines.h"
#include <Stepper.h>
#include <Wire.h> 
#include <VL6180X.h>

int Stap = 0;
float Distance = 0;
int Koers = 0;
int Bocht = -1;
//int Bochten_Patroon[] = [Linksom, Rechtsom, Linksom, Linksom, Rechtsom, Linksom];

Stepper Stepper_Links(STEPS, Pin_Links_Ain2, Pin_Links_Ain1, Pin_Links_Bin1, Pin_Links_Bin2);
Stepper Stepper_Rechts(STEPS, Pin_Rechts_Ain2, Pin_Rechts_Ain1, Pin_Rechts_Bin1, Pin_Rechts_Bin2);


//float Distance_Cal(int trigPin, int echoPin);
//void Bocht_linksom();
//void Bocht_rechtsom();

void setup()
{
  Serial.begin(9600);
  Stepper_Links.setSpeed(120);
  Stepper_Rechts.setSpeed(120);
    //pinModes Ultrasoon
  pinMode(Ultrasoon_Voor_Trigger, OUTPUT);
  pinMode(Ultrasoon_Voor_Echo, INPUT);
  pinMode(Ultrasoon_Links_Voor_Trigger, OUTPUT);
  pinMode(Ultrasoon_Links_Voor_Echo, INPUT);
  pinMode(Ultrasoon_Rechts_Voor_Trigger, OUTPUT);
  pinMode(Ultrasoon_Rechts_Voor_Echo, INPUT);
  pinMode(Ultrasoon_Links_Achter_Trigger, OUTPUT);
  pinMode(Ultrasoon_Links_Achter_Echo, INPUT);
  pinMode(Ultrasoon_Rechts_Achter_Trigger, OUTPUT);
  pinMode(Ultrasoon_Rechts_Achter_Echo, INPUT);

  pinMode(Standby_Pin_Rechts, OUTPUT);
  pinMode(Standby_Pin_Links, OUTPUT);
  digitalWrite(Standby_Pin_Rechts, HIGH);
  digitalWrite(Standby_Pin_Links, HIGH);
  
  //Overige Pinmodes
//  pinMode(Signaal_Ledjes, OUTPUT);
//  pinMode(Shut_ToF_Rechts,OUTPUT);
//  pinMode(Shut_ToF_Links,OUTPUT);

  //Digital Writes
  digitalWrite(Ultrasoon_Voor_Trigger, LOW);
  digitalWrite(Ultrasoon_Links_Voor_Trigger, LOW);
  digitalWrite(Ultrasoon_Rechts_Voor_Trigger, LOW);
  digitalWrite(Ultrasoon_Links_Achter_Trigger, LOW);
  digitalWrite(Ultrasoon_Rechts_Achter_Trigger, LOW);
}

void loop()
{
  switch(Stap)
  {
    case (Idle):
      digitalWrite(Standby_Pin_Links, LOW);
      digitalWrite(Standby_Pin_Rechts, LOW);
      break;
    case (Rijden):
      //digitalWrite(LedPins, LOW);
      digitalWrite(Standby_Pin_Rechts, HIGH);
      digitalWrite(Standby_Pin_Links, HIGH);
      Stepper_Links.step(1);
      Stepper_Rechts.step(1);
      break;
    case (Actie_Proces_Gewas):
      //digitalWrite(LedPins, HIGH);
      digitalWrite(Standby_Pin_Links, LOW);
      digitalWrite(Standby_Pin_Rechts, LOW);
      delay(2000);
      
      break;
      
  }
  
  switch(Stap)
  {

    case (Idle): //------------------------------------------------------------------
      Stap = Rijden;
      break;
      
    case (Rijden): //--------------------------------------------------------------------
      Distance = Distance_Cal(Ultrasoon_Links_Achter_Trigger, Ultrasoon_Links_Achter_Echo);
      if(Distance < Gewas_Afstand && Distance > 1)
      {
        Stap = Actie_Proces_Gewas;
      }
      Distance = Distance_Cal(Ultrasoon_Rechts_Achter_Trigger, Ultrasoon_Rechts_Achter_Echo);
      if(Distance < Gewas_Afstand && Distance > 1)
      {
        Stap = Actie_Proces_Gewas;
      }

      break;

    case (Actie_Proces_Gewas): //-------------------------------------------------------------
      Stap = Rijden;
      break;
  }
}

float Distance_Cal(int trigPin, int echoPin)
{
  long duration;
  long distance;
  digitalWrite(echoPin, LOW);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH, 3000);
  distance = duration * 0.34/2;
  digitalWrite(trigPin, LOW);
  return distance;
}
