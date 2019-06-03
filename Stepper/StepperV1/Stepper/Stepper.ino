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
#define Rpin_ain2  28
#define Rpin_ain1  29
#define Rpin_bin1  30
#define Rpin_bin2  31

#define Motor_speed_max     120                 //100rpm is max reacheble speed op 5V
#define Motor_speed_follow  (Motor_speed_max/7) //70% speed for following person
#define Motor_speed_half    (Motor_speed_max/2)
#define Motor_speed_stop    0                   //Set motor stil but is still running
#define Aantal_rond         2*STEPS
#define Aantal_rond         3*STEPS


Stepper stepper_links(STEPS, Lpin_ain2, Lpin_ain1, Lpin_bin1, Lpin_bin2);
Stepper stepper_rechts(STEPS, Rpin_ain2, Rpin_ain1, Rpin_bin1, Rpin_bin2);


//void Bocht(value voor bocht links of rechts);
//void Volg_Modus();
//void Gewas();

int val = 0;
int i = 0;
void setup()
{

  Serial.begin(9600);
  Serial.println("Stepper test!");

  // set the speed of the motor to 30 RPMs
  

}

void loop()
{
  stepper_links.setSpeed(120);
  stepper_rechts.setSpeed(120);
 //for(i = 0; i<Aantal_rond; i++) //laat hopelijk beide wielen tegelijk rijden
 //{
  stepper_links.step(1);
  stepper_rechts.step(1);
  //delay(0); // max delay tussen steps around 6~7 miliseconde
 //}
}
/*

void Bocht(value voor bocht links of rechts)
{
  //ToF sensoren uit
  if(bocht == rechts)                         //Bij een bocht naar links of rechts wordt de binnenste motor op 50% gezet
  {
    stepper_links.setSpeed(Motor_speed_half);
    stepper_rechts.setSpeed(Motor_speed_max);
    
    for(i = 0; i<Aantal_rond_bocht; i++)
    {
      stepper_links.step(2);      //Het linker wiel moet een grotere afstand af leggen. gekozen voor 3x zo groot
      stepper_rechts.step(1);
    }
  }

  if(bocht == links)
  {
    //Bochten ++
    stepper_links.setSpeed(Motor_speed_half);
    stepper_rechts.setSpeed(Motor_speed_max);
    for(i = 0; i<Aantal_rond_bocht; i++)
    {
    stepper_rechst.step(2);      //Het rechter wiel moet een grotere afstand af leggen. gekozen voor 3x zo groot
    stepper_links.step(1);
    }
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

*/
