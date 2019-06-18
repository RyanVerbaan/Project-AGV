#include "Defines.h"
#include "Functies.h"
#include <Stepper.h>
#include <Wire.h> 
#include <VL6180X.h>

int Stap = 0;
float Distance = 0;
int Koers = 0;
int Bocht = 0;
int Bochten_Patroon[6] = [Linksom, Rechtsom, Linksom, Linksom, Rechtsom, Linksom];

Stepper Stepper_Links(STEPS, Pin_Links_Ain2, Pin_Links_Ain1, Pin_Links_Bin1, Pin_Links_Bin2);
Stepper Stepper_Rechts(STEPS, Pin_Rechts_Ain2, Pin_Rechts_Ain1, Pin_Rechts_Bin1, Pin_Rechts_Bin2);

float Distance_Cal(int trigPin, int echoPin);
void Init();
void Bocht_Linksom();
void Bocht_Rechtsom();

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  switch(Stap)
  {
    case (Volgmodus):
      break;
    case (Idle):
      Stepper_Links.step(0);
      Stepper_Rechts.step(0);
      break;
    case (Rijden):
      Stepper_Links.step(1);
      Stepper_Rechts.step(1);
      break;
    case (Actie_Proces_Gewas):
      Stepper_Links.step(0);
      Stepper_Rechts.step(0);
      digitalWrite(LedPins, HIGH);
      delay(2000);
      digitalWrite(LedPins, LOW);
      break;
    case (Actie_Proces_Object):
      Stepper_Links.step(0);
      Stepper_Rechts.step(0);
      break;
    case (Actie_Proces_Koers):
      break;
  }
  
  switch(Stap)
  {
    case (Volgmodus): //----------------------------------------------------------
      break;
      
    case (Idle): //------------------------------------------------------------------
      if(digitalRead(Volgmodus_Pin) == HIGH)
      {
        Stap = Volgmodus;
      }
      if(digitalRead(Autonoom_Pin == HIGH)
      {
        Stap = Rijden;
      }
      break;
      
    case (Rijden): //--------------------------------------------------------------------
      if(digitalRead(Autonoom_Pin != HIGH)
      {
        Stap = Idle;
      }

      Distance = Distance_Cal(Ultrasoon_Voor_Trigger, Ultrasoon_Voor_Echo);
      if(Distance < ArmLengte && Distance > 0)
      {
        Stap = Actie_Proces_Object;
      }

      Distance = Distance_Cal(Ultrasoon_Links_Achter_Trigger, Ultrasoon_Links_Achter_Echo);
      if(Distance < Gewas_Afstand && Distance > 0)
      {
        Stap = Actie_Proces_Gewas;
      }

      Distance = Distance_Cal(Ultrasoon_Rechts_Achter_Trigger, Ultrasoon_Rechts_Achter_Echo);
      if(Distance < Gewas_Afstand && Distance > 0)
      {
        Stap = Actie_Proces_Gewas;
      }

      if(digitalRead(Eindstand_Pin_Rechts) == LOW)
      {
        Stap = Actie_Proces_Koers;
      }

      if(digitalRead(Eindstand_Pin_Links) == LOW)
      {
        Stap = Actie_Proces_Koers;
      }
      break;
      
    case (Actie_Proces_Gewas): //-------------------------------------------------------------
      Stap = Rijden;
      break;
      
    case (Actie_Proces_Object): //--------------------------------------------------------------
      Distance = Distance_Cal(Ultrasoon_Voor_Trigger,Ultrasoon_Voor_Echo);
      if(Distance > ArmLengte && Distance > 0)
      {
        Stap = Rijden;
      }
      break;
      
    case (Actie_Proces_Koers): //----------------------------------------------------------------
      if(Bocht == Linksom)
      {
        Bocht_Linksom(); 
      }
      if(Bocht == Rechtsom)
      {
        Bocht_Rechtsom
      }
      if(digitalRead(Eindstand_Pin_Links) == HIGH)
      {
        Stap == Rijden;
      }
      if(digitalRead(Eindstand_Pin_Rechts) == HIGH)
      {
        Stap == Rijden;
      }
      break;
      
  }
}

void Bocht_Rechtsom()
{
  for (int r;r<10;r++)
  {
   Stepper_Links.step(1);
   Stepper_Rechts.step(1);
   Stepper_Links.step(1); 
  }
}
void Bocht_Linksom()
{
  for (int r;r<10;r++)
  {
   Stepper_Rechts.step(1);
   Stepper_Links.step(1);
   Stepper_Rechts.step(1); 
  }
}





void Init()
{
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

  //Overige Pinmodes
  pinMode(Signaal_Ledjes, OUTPUT);
  pinMode(Shut_ToF_Rechts,OUTPUT);
  pinMode(Shut_ToF_Links,OUTPUT);

  //Digital Writes
  digitalWrite(Ultrasoon_Voor_Trigger, LOW);
  digitalWrite(Ultrasoon_Links_Voor_Trigger, LOW);
  digitalWrite(Ultrasoon_Rechts_Voor_Trigger, LOW);
  digitalWrite(Ultrasoon_Links_Achter_Trigger, LOW);
  digitalWrite(Ultrasoon_Rechts_Achter_Trigger, LOW);
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
