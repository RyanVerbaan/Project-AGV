#include <Stepper.h>

//Defines voor stepper motor(A_Mega)
//Aantal steps op de motor
#define STEPS     200
//Pinnen voor linker stepper  //Mogen digitale pinnen zijn
#define Lpin_ain2  22
#define Lpin_ain1  23
#define Lpin_bin1  24
#define Lpin_bin2  25
//Pinnen voor retcher stepper
#define Rpin_ain1  1
#define Rpin_ain2  0
#define Rpin_bin1  2
#define Rpin_bin2  3

#define Motor_speed_max     100                 //100rpm is max reacheble speed op 5V
#define Motor_speed_follow  (Motor_speed_max/7) //70% speed for following person
#define Motor_speed_half    (Motor_speed_max/2)
#define Motor_speed_stop    0                   //Set motor stil but is still running



Stepper stepper_links(STEPS, Lpin_ain2, Lpin_ain1, Lpin_bin1, Lpin_bin2);
Stepper stepper_rechts(STEPS, Rpin_ain2, Rpin_ain1, Rpin_bin1, Rpin_bin2);


void Bocht(value voor bocht links of rechts);
void Volg_Modus();
void Gewas();

int val = 0;

void setup()
{

  Serial.begin(9600);
  Serial.println("Stepper test!");

  // set the speed of the motor to 30 RPMs
  stepper_links.setSpeed(60);
  stepper_links.step(200);

}

void loop()
{

}

void Bocht(value voor bocht links of rechts)
{
  //ToF sensoren uit
  if(bocht == rechts)                         //Bij een bocht naar links of rechts wordt de binnenste motor op 50% gezet
  {
    stepper_links.setSpeed(Motor_speed_half);
    stepper_rechts.setSpeed(Motor_speed_max);
    stepper_links.step(5*STEPS);            //Het buitenste wiel moet een grotere afstand afleggen
    stepper_rechts.step(4*STEPS);
  }

  if(bocht == links)
  {
    //Bochten ++
    stepper_links.setSpeed(Motor_speed_half);
    stepper_rechts.setSpeed(Motor_speed_max);
    stepper_links.step(4*STEPS);
    stepper_rechts.step(5*STEPS);
  }
}

void Volg_Modus()
{
  stepper_links.setSpeed(Motor_speed_follow);
  stepper_rechts.setSpeed(Motor_speed_follow);
  //if statements met versnellen van de motor/vertragen moeten hier komen
}

void Gewas()
{
  stepper_links.setSpeed(Motor_speed_stop);
  stepper_rechts.setSpeed(Motor_speed_stop);
}
