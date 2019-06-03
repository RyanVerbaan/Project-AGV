#include <Stepper.h>

//Aantal steps op de motor
#define STEPS     200
//Pinnen voor linker stepper
#define Lpin_ain2  22
#define Lpin_ain1  23
#define Lpin_bin1  24
#define Lpin_bin2  25
//Pinnen voor retcher stepper
#define Rpin_ain2  26
#define Rpin_ain1  27
#define Rpin_bin1  28
#define Rpin_bin2  29

#define Ultrasoon_Voor_Trigger 12
#define Ultrasoon_Links_Trigger 6
#define Ultrasoon_Rechts_Trigger 4

#define Ultrasoon_Voor_Echo 11
#define Ultrasoon_Links_Echo 5
#define Ultrasoon_Rechts_Echo 3


#define Rijden 0
#define Bocht_Rechtsom 1
#define Bocht_Linksom 2
int stap = 0;
int Distance = 0;

Stepper stepper_links(STEPS, Lpin_ain2, Lpin_ain1, Lpin_bin1, Lpin_bin2);
Stepper stepper_rechts(STEPS, Rpin_ain2, Rpin_ain1, Rpin_bin1, Rpin_bin2);

int Distance_Cal(int trigPin, int echoPin)
{
  float duration;
  float distance;
  float gem_distance;
  float tot_distance = 0;
  int i = 0;
  for(i = 0; i < 10; i++)
  {
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = duration* 0.34/2; 
    if(distance > 0)
    {
      tot_distance = tot_distance + distance;
    }
    else
    {
      i--;
    }
  }
  gem_distance = tot_distance / i;
  return gem_distance;
}

//Deze functie is voor als de bovengeschreven functie te langzaam is
//int Distance_Cal(int trigPin, int echoPin) 
//{
//  float duration;
//  float distance;
//  digitalWrite(trigPin, HIGH);
//  delayMicroseconds(10);
//  digitalWrite(trigPin, LOW);
//  duration = pulseIn(echoPin, HIGH);
//  distance = duration* 34/2;
//  return distance;
//}

void setup()
{
  Serial.begin(9600);
  Serial.println("Stepper test!");
  stepper_links.setSpeed(120);
  stepper_rechts.setSpeed(120);

  pinMode(Ultrasoon_Voor_Trigger, OUTPUT);
  pinMode(Ultrasoon_Voor_Echo, INPUT);
  pinMode(Ultrasoon_Links_Trigger, OUTPUT);
  pinMode(Ultrasoon_Links_Echo, INPUT);
  pinMode(Ultrasoon_Rechts_Trigger, OUTPUT);
  pinMode(Ultrasoon_Rechts_Echo, INPUT);

  digitalWrite(Ultrasoon_Voor_Trigger, LOW);
  digitalWrite(Ultrasoon_Links_Trigger, LOW);
  digitalWrite(Ultrasoon_Rechts_Trigger, LOW);
}

void loop()
{
  switch(stap)
  {
     case (Rijden):
        stepper_links.step(1);
        stepper_rechts.step(1);
        Distance = Distance_Cal(Ultrasoon_Links_Trigger, Ultrasoon_Links_Echo);
        if(Distance < 80);
          stap = Bocht_Linksom;
        Distance = Distance_Cal(Ultrasoon_Rechts_Trigger, Ultrasoon_Rechts_Echo);
        if(Distance < 80);
          stap = Bocht_Rechtsom;
      break;
    case (Bocht_Rechtsom):
      stepper_links.step(2);
      stepper_rechts.step(1);
      Distance = Distance_Cal(Ultrasoon_Voor_Trigger, Ultrasoon_Voor_Echo);
      if(Distance < 80);
        stap = Rijden;
      break;
    case (Bocht_Linksom):
      stepper_links.step(1);
      stepper_rechts.step(2);
      Distance = Distance_Cal(Ultrasoon_Voor_Trigger, Ultrasoon_Voor_Echo);
      if(Distance < 80);
        stap = Rijden;
      break; 
  }
}
