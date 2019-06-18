#include "Defines.h"
#include <Stepper.h>

int Linksom  = 2;
int Rechtsom = 1;

int Stap = 0;
float Distance = 0;
int Koers = 0;
int Bocht = -1;
int Bochten_Patroon[] = {Linksom, Rechtsom, Linksom, Linksom, Rechtsom, Linksom};

Stepper Stepper_Links(STEPS, Pin_Links_Ain2, Pin_Links_Ain1, Pin_Links_Bin1, Pin_Links_Bin2);
Stepper Stepper_Rechts(STEPS, Pin_Rechts_Ain2, Pin_Rechts_Ain1, Pin_Rechts_Bin1, Pin_Rechts_Bin2);


float Distance_Cal(int trigPin, int echoPin);
void Bocht_linksom();
void Bocht_rechtsom();
void Klein_Stukje_Rijden();

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
  pinMode(Eindstand_Pin_Links_Groot, INPUT_PULLUP);
  pinMode(Eindstand_Pin_Rechts_Groot, INPUT_PULLUP);
  pinMode(Standby_Pin_Rechts, OUTPUT);
  pinMode(Standby_Pin_Links, OUTPUT);
  

  //Digital Writes
  digitalWrite(Standby_Pin_Rechts, HIGH);
  digitalWrite(Standby_Pin_Links, HIGH);
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
      digitalWrite(Standby_Pin_Rechts, HIGH);
      digitalWrite(Standby_Pin_Links, HIGH);
      for (int r=0;r<20;r++)
      {
       Stepper_Rechts.step(1);
       Stepper_Links.step(1); 
      }
      break;
    case (Actie_Proces_Gewas):
      digitalWrite(Standby_Pin_Links, LOW);
      digitalWrite(Standby_Pin_Rechts, LOW);
      delay(500);
      digitalWrite(Standby_Pin_Rechts, HIGH);
      digitalWrite(Standby_Pin_Links, HIGH);
      
      digitalWrite(LedPins, HIGH);
      for(int r=0; r<200;r++)
      {
      Stepper_Rechts.step(1);
      Stepper_Links.step(1);
      }
      digitalWrite(LedPins, LOW);
      digitalWrite(Standby_Pin_Links, LOW);
      digitalWrite(Standby_Pin_Rechts, LOW);
      delay(2000);
      break;
    case (Actie_Proces_Object):
      digitalWrite(Standby_Pin_Links, LOW);
      digitalWrite(Standby_Pin_Rechts, LOW);
      delay(5);
      break;
    case (Actie_Proces_Koers):
      Bocht++;
      if(Bocht == 6)
      {
        Bocht = 0;
      }
      break;
  }
  
  switch(Stap)
  {

    case (Idle): //------------------------------------------------------------------
      Stap = Rijden;
      break;
      
    case (Rijden): //--------------------------------------------------------------------
      Distance = Distance_Cal(Ultrasoon_Voor_Trigger, Ultrasoon_Voor_Echo);
      if(Distance < ArmLengte && Distance > 5)
      {
        Stap = Actie_Proces_Object;
      }
      Stepper_Rechts.step(1);
      Stepper_Links.step(1);
      Distance = Distance_Cal(Ultrasoon_Links_Achter_Trigger, Ultrasoon_Links_Achter_Echo);
      if(Distance < Gewas_Afstand && Distance > 1)
      {
        Stap = Actie_Proces_Gewas;
      }
      Stepper_Rechts.step(1);
      Stepper_Links.step(1);
      Distance = Distance_Cal(Ultrasoon_Rechts_Achter_Trigger, Ultrasoon_Rechts_Achter_Echo);
      if(Distance < Gewas_Afstand && Distance > 1)
      {
        Stap = Actie_Proces_Gewas;
      }
//      if((digitalRead(Eindstand_Pin_Rechts_Groot) == HIGH) && (digitalRead(Eindstand_Pin_Links_Groot) == HIGH))
//      {
//        Stap = Actie_Proces_Koers;
//      }
      break;

    case (Actie_Proces_Gewas): //-------------------------------------------------------------
      Distance = Distance_Cal(Ultrasoon_Rechts_Achter_Trigger, Ultrasoon_Rechts_Achter_Echo);      
      if(Distance > Gewas_Afstand && Distance > 5)
      {
        Stap = Rijden;
      }
      break;
      
    case (Actie_Proces_Object): //--------------------------------------------------------------
      Distance = Distance_Cal(Ultrasoon_Voor_Trigger, Ultrasoon_Voor_Echo);
      if(Distance > ArmLengte && Distance > 5)
      {
        Stap = Rijden;
      }
        

      break;
    case (Actie_Proces_Koers): //----------------------------------------------------------------
      if(Bochten_Patroon[Bocht] == Linksom)
      {
        Bocht_linksom(); 
      }
      if(Bochten_Patroon[Bocht] == Rechtsom)
      {
        Bocht_rechtsom();
      }
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

void Bocht_rechtsom()
{
  for (int r=0;r<50;r++)
  {
   Stepper_Links.step(1);
   Stepper_Rechts.step(1);
   Stepper_Links.step(1); 
  }
}

void Bocht_linksom()
{
  for (int r=0;r<50;r++)
  {
   Stepper_Rechts.step(1);
   Stepper_Links.step(1);
   Stepper_Rechts.step(1); 
  }
}

void Klein_Stukje_Rijden()
{
  for(int r=0;r<200;r++)
  {
    Stepper_Links.step(1);
    Stepper_Rechts.step(1);
  }
}
