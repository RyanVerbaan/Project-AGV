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
#define GIY_Test

#define Motor_speed_max     120                 //100rpm is max reacheble speed op 5V
#define Motor_speed_follow  (Motor_speed_max/7) //70% speed for following person
#define Motor_speed_half    (Motor_speed_max/2)
#define Motor_speed_stop    0                   //Set motor stil but is still running

Stepper stepper_links(STEPS, Pin_Links_Ain2, Pin_Links_Ain1, Pin_Links_Bin1, Pin_Links_Bin2);
Stepper stepper_rechts(STEPS, Pin_Rechts_Ain2, Pin_Rechts_Ain1, Pin_Rechts_Bin1, Pin_Rechts_Bin2);




//void Bocht(value voor bocht links of rechts);
//void Volg_Modus();
//void Gewas();

//int val = 0;
//int i = 0;
void setup()
{

  Serial.begin(9600);
  Serial.println("Stepper test!");
  stepper_links.setSpeed(120);
  stepper_rechts.setSpeed(120);
  Serial.println("Stepper test klaar!");
  delay(1000);

}

void loop()
{

  stepper_links.step(1);
  stepper_rechts.step(1);
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
