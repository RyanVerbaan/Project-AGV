#include <Stepper.h>

//Defines voor stepper motor(A_Mega)

// links: w b - rz z
// rechst: z g - rb w
//Aantal steps op de motor
#define STEPS     200
//Pinnen voor linker stepper  //Mogen digitale pinnen zijn
#define Pin_Links_Ain2  40
#define Pin_Links_Ain1  41
#define Pin_Links_Bin1  44
#define Pin_Links_Bin2  45
//Pinnen voor retcher stepper
#define Pin_Rechts_Ain2  32
#define Pin_Rechts_Ain1  33
#define Pin_Rechts_Bin1  34
#define Pin_Rechts_Bin2  35
<<<<<<< HEAD
#define hoi 
=======
#define GIY_Test
>>>>>>> c1d75afbbd235702cf12210ad1fc1a91cbae5fd8

#define STBY_Rechts 29
#define STBY_Links 28

#define Motor_speed_max     120                 //100rpm if max reacheble speed op 5V
#define Motor_speed_follow  (Motor_speed_max/7) //70% speed for following person
#define Motor_speed_half    (Motor_speed_max/2)
#define Motor_speed_stop    0                   //Set motor stil but is still running
#define Aantal_Steps 100

#define Ultrasoon_Voor_Trigger 12
#define Ultrasoon_Links_Voor_Trigger 10
#define Ultrasoon_Rechts_Voor_Trigger 8
#define Ultrasoon_Links_Achter_Trigger 6
#define Ultrasoon_Rechts_Achter_Trigger 4

#define Ultrasoon_Voor_Echo 11
#define Ultrasoon_Links_Voor_Echo 9
#define Ultrasoon_Rechts_Voor_Echo 7
#define Ultrasoon_Links_Achter_Echo 5
#define Ultrasoon_Rechts_Achter_Echo 3


Stepper stepper_links(STEPS, Pin_Links_Ain2, Pin_Links_Ain1, Pin_Links_Bin1, Pin_Links_Bin2);
Stepper stepper_rechts(STEPS, Pin_Rechts_Ain2, Pin_Rechts_Ain1, Pin_Rechts_Bin1, Pin_Rechts_Bin2);

float Distance = 0;

void setup()
{
  Serial.begin(9600);
<<<<<<< HEAD
  stepper_links.setSpeed(100);
  stepper_rechts.setSpeed(100);

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

  pinMode(STBY_Rechts, OUTPUT);
  pinMode(STBY_Links, OUTPUT);
  
  digitalWrite(Ultrasoon_Voor_Trigger, LOW);
  digitalWrite(Ultrasoon_Links_Voor_Trigger, LOW);
  digitalWrite(Ultrasoon_Rechts_Voor_Trigger, LOW);
  digitalWrite(Ultrasoon_Links_Achter_Trigger, LOW);
  digitalWrite(Ultrasoon_Rechts_Achter_Trigger, LOW);
=======
  Serial.println("Stepper test!");
  stepper_links.setSpeed(120);
  stepper_rechts.setSpeed(120);
  digitalWrite(28, HIGH);
  digitalWrite(29, HIGH);
  Serial.println("Stepper test klaar!");
  delay(1000);

>>>>>>> a1417c0097e900a584914c36a454a6e71d0b742f
}

void loop()
{
<<<<<<< HEAD

  Distance = Distance_Cal(Ultrasoon_Links_Achter_Trigger, Ultrasoon_Links_Achter_Echo);
  //Serial.println(Distance);
  if(Distance < 100 && Distance > 1)
=======
  stepper_links.step(1);
  delay(2);
  stepper_rechts.step(1);
  delay(2);
}
/*
void Bocht(value voor bocht links of rechts)
{
  //ToF sensoren uit
  if(bocht == rechts)                         //Bij een bocht naar links of rechts wordt de binnenste motor op 50% gezet
>>>>>>> a1417c0097e900a584914c36a454a6e71d0b742f
  {
    digitalWrite(STBY_Rechts, HIGH);
    digitalWrite(STBY_Links, HIGH);
    stepper_links.step(1);
    stepper_rechts.step(1);
  }
  else
  {
    digitalWrite(STBY_Rechts, LOW);
    digitalWrite(STBY_Links, LOW);
  }
}


int Distance_Cal(int trigPin, int echoPin)
{
  float duration;
  float distance;
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration* 0.34/2;
  return distance;
}
