#include <Stepper.h> 

//Defines voor stepper motor(A_Mega)
  //Aantal steps op de motor
#define STEPS     200
  //Pinnen voor linker stepper
#define Lpin_ain1  4
#define Lpin_ain2  5
#define Lpin_bin1  6
#define Lpin_bin2  7
  //Pinnen voor retcher stepper
#define Rpin_ain1  28
#define Rpin_ain2  29
#define Rpin_bin1  30
#define Rpin_bin2  31

#define knopje     A8


Stepper stepper_links(STEPS, Lpin_ain2, Lpin_ain1, Lpin_bin1, Lpin_bin2);
Stepper stepper_rechts(STEPS, Rpin_ain2, Rpin_ain1, Rpin_bin1, Rpin_bin2);
 
int val = 0;

void setup()
{
  
  pinMode(knopje, INPUT);
  Serial.begin(9600);
  Serial.println("Stepper test!");
  
  // set the speed of the motor to 30 RPMs
  stepper_links.setSpeed(60);
  
}
 
void loop()
{
  stepper_links.step(1);
  val = analogRead(knopje);
  Serial.println(analogRead(knopje));
  if(val >= 900)
  {
    stepper_links.setSpeed(60);
  }
  else{
    stepper_links.setSpeed(5);
  }
  delay(1);
  //Serial.println("Backward");
  //stepper.step(-STEPS);
}
