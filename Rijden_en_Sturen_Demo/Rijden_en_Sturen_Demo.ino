#include <Stepper.h>


// links: w b - rz z
// rechst: z g - rd w
//Aantal steps op de motor
#define STEPS     200
//Pinnen voor linker stepper  //Mogen digitale pinnen zijn
#define Lpin_ain2  28
#define Lpin_ain1  29
#define Lpin_bin1  30
#define Lpin_bin2  31
//Pinnen voor retcher stepper
#define Rpin_ain2  36
#define Rpin_ain1  37
#define Rpin_bin1  38
#define Rpin_bin2  39

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
float Distance = 0;
int teller = 0;

Stepper stepper_links(STEPS, Lpin_ain2, Lpin_ain1, Lpin_bin1, Lpin_bin2);
Stepper stepper_rechts(STEPS, Rpin_ain2, Rpin_ain1, Rpin_bin1, Rpin_bin2);

//int Distance_Cal(int trigPin, int echoPin)
//{
//  float duration = 0;
//  float distance = 0;
//  float gem_distance = 0;
//  float tot_distance = 0;
//  int i = 0;
//  for (i = 0; i < 10; i++)
//  {
//    delayMicroseconds(2);
//    digitalWrite(trigPin, LOW);
//    digitalWrite(trigPin, HIGH);
//    delayMicroseconds(10);
//    digitalWrite(trigPin, LOW);
//    duration = pulseIn(echoPin, HIGH);
//    distance = duration * 0.34 / 2;
//    Serial.print("distance: ");
//    Serial.println(distance);
//    if (distance > 0)
//    {
//      tot_distance += distance;
//    }
//    else
//    {
//      i--;
//      //      Serial.print("Else statement distance_Cal");
//      //      Serial.println(tot_distance);
//      //      delay(500);
//    }
//    //  delay(50);
//  }
//  gem_distance = tot_distance / i;
//  return gem_distance;
//}

//Deze functie is voor als de bovengeschreven functie te langzaam is
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

void setup()
{
//  Serial.begin(9600);
//  Serial.println("Stepper test!");
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
  delay(5000);
}

void loop()
{
  teller++;
//  Serial.print("stap: "); Serial.println(stap);
  switch (stap)
  {
    case (Rijden):
      stepper_links.step(1);
      stepper_rechts.step(1);
      if(teller > 20)
      {
        Distance = Distance_Cal(Ultrasoon_Links_Trigger, Ultrasoon_Links_Echo);
      }
      
      Serial.print("distance links: ");
      Serial.println(Distance);
      if (Distance < 80)
      {
        stap = Bocht_Linksom; 
      }
      if(teller > 20)
      {
        Distance = Distance_Cal(Ultrasoon_Rechts_Trigger, Ultrasoon_Rechts_Echo);
      } 
      if(teller > 20)
      {
        teller = 0;
      } 
      
      if(Distance < 80)
      {
        stap = Bocht_Rechtsom;
      }  
      break;
      
    case (Bocht_Rechtsom):
      stepper_links.step(3);
      stepper_rechts.step(1);
      if(teller > 10)
      {
        Distance = Distance_Cal(Ultrasoon_Voor_Trigger, Ultrasoon_Voor_Echo);
        teller = 0;
      } 
      if (Distance < 80)
      {
        stap = Rijden;
      }
      break;
      
    case (Bocht_Linksom):
      stepper_links.step(1);
      stepper_rechts.step(3);
      if(teller > 10)
      {
        Distance = Distance_Cal(Ultrasoon_Voor_Trigger, Ultrasoon_Voor_Echo);
        teller = 0;
      } 
      if (Distance < 80)
      {
        stap = Rijden;
      }
      break;
  }
}
