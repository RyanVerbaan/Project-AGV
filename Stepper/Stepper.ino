#include <Stepper.h> 

//Defines voor stepper motor(A_Mega)
  //Aantal steps op de motor
#define STEPS     200
  //Pinnen voor linker stepper
#define Lpin_ain1  22
#define Lpin_ain2  23
#define Lpin_bin1  24
#define Lpin_bin2  25
  //Pinnen voor retcher stepper
#define Rpin_ain1  28
#define Rpin_ain2  29
#define Rpin_bin1  30
#define Rpin_bin2  31

Stepper stepper_links(STEPS, Lpin_ain2, Lpin_ain1, Lpin_bin1, Lpin_bin2);
Stepper stepper_rechts(STEPS, Rpin_ain2, Rpin_ain1, Rpin_bin1, Rpin_bin2);
 
 
void setup()
{
  Serial.begin(9600);
  Serial.println("Stepper test!");
  
  // set the speed of the motor to 30 RPMs
  stepper_links.setSpeed(60);
  
}
 
void loop()
{
  Serial.println("Forward");
  stepper_links.step(STEPS);
  //Serial.println("Backward");
  //stepper.step(-STEPS);
}
