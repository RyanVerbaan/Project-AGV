#include <Stepper.h>

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

#define Volg_Modus -1
#define Idle 0
#define Rijden 1
#define Actie_Proces_Gewas 3
#define Actie_Proces_Obstakel 2
#define Actie_Proces_Koers 4

#define Autonoom -1
#define Volgmodus 1

#define Rij_Snelheid 512
#define Arm_Lengte 875 //milimeter 1/8 Schaalmodel van 0,7 Meter

int Stap = 0;
int Status = 0;

#define Stepper_Links_Pin
#define Stepper_Rechts_Pin
#define Ultrasoon_Voor
#define Ultrasoon_Links_Voor
#define Ultrasoon_Rechts_Voor
#define Ultrasoon_Links_Achter
#define Ultrasoon_Rechts_Achter
#define Time_Of_Flight_Links
#define Time_Of_Flight_Rechts
#define Signaal_Ledjes


Stepper stepper_links(STEPS, Lpin_ain2, Lpin_ain1, Lpin_bin1, Lpin_bin2);
Stepper stepper_rechts(STEPS, Rpin_ain2, Rpin_ain1, Rpin_bin1, Rpin_bin2);

void setup() 
{  
  pinMode(Ultrasoon_Voor, INPUT);
  pinMode(Ultrasoon_Links_Voor, INPUT);
  pinMode(Ultrasoon_Rechts_Voor, INPUT);
  pinMode(Ultrasoon_Links_Achter, INPUT);
  pinMode(Ultrasoon_Rechts_Achter, INPUT);
  
  pinMode(Time_Of_Flight_Links, INPUT);//AANPASSEN? ToF links en rechts op zelfde pin met switches die omschakelen na het geheel uitvoeren van een bochtfunctie
  pinMode(Time_Of_Flight_Rechts, INPUT);
  
  pinMode(Signaal_Ledjes, OUTPUT);
  
  stepper_links.setSpeed(Motor_speed_stop);
  stepper_rechts.setSpeed(Motor_speed_stop);
}
  
void Actie_Proces_Gewas()
  {
    stepper_links.setSpeed(Motor_speed_stop);
    stepper_rechts.setSpeed(Motor_speed_stop);
    delay(100);
    digitalWrite(Signaal_Ledjes, HIGH);
    delay(100);
    digitalWrite(Signaal_Ledjes, LOW);
  }
  
void Actie_Proces_Obstakel()
  {
    stepper_links.setSpeed(Motor_speed_stop);
    stepper_rechts.setSpeed(Motor_speed_stop);
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
  
  
void Volg_Modus(waarden_ultrasoon)
  {
  stepper_links.setSpeed(Motor_speed_follow);
  stepper_rechts.setSpeed(Motor_speed_follow);
  if(Waarden_ultrasoon>waarden_binnen_marge)
  {
    stepper_links.setSpeed(Motor_speed_follow-5);
    stepper_rechts.setSpeed(Motor_speed_follow-5);
  }
  if(Waarden_ultrasoon>waarden_binnen_marge)
  {
    stepper_links.setSpeed(Motor_speed_follow+5);
    stepper_rechts.setSpeed(Motor_speed_follow+5);
  }
  }


void loop() {
  switch(Stap) //Switchcase met states
  {
    case (Idle): 
      //Motoren uit en wachten
      stepper_links.setSpeed(Motor_speed_stop);
      stepper_rechts.setSpeed(Motor_speed_stop);
      break;
      
    case (Rijden):
      //Steppers op standaard rijsnelheid
      stepper_links.setSpeed(Motor_speed_max);
      stepper_rechts.setSpeed(Motor_speed_max);
      break;
      
    case (Actie_Proces_Gewas):
      //Stappenmotoren stilzetten en signaal afgeven
      Actie_Proces_Gewas();
      break;
      
    case (Actie_Proces_Obstakel): //Actie_Proces_Bocht?
      //Gepaste Afstand houden --> Stilstaan of langzamer gaan rijden.
      Bocht(value voor links of rechts);
      break; 
          
    case (Actie_Proces_Koers):
      //Bijsturen en Bochtnemen
      Actie_Proces_Koers();
      break;
      
    case (Volg_Modus):
      //Rijden, Stoppen en Bijsturen.
      Volg_Modus();
      break;
  }

  switch(Stap) //Switchcase met overgangen
  {
    case (Idle):
      if(Status == Autonoom) //Knop?
      {
        Stap = Rijden;
      }
      if(Status == Volgmodus) //Knop?
      {
        Stap = Volg_Modus;
      }
      break;
      
    case (Rijden):
      if(Status != Autonoom)
      Stap = Idle;

      if(Ultrasoon_Links_Achter == HIGH || Ultrasoon_Rechts_Achter == HIGH); //moet nog vergeleken met een waarde maar niet op HIGH
      Stap = Actie_Proces_Gewas;
      
      if(Ultrasoon_Links_Voor == HIGH || Ultrasoon_Voor == HIGH || Ultrasoon_Rechts_Voor == HIGH); //moet nog vergeleken met een waarde maar niet op HIGH
      Stap = Actie_Proces_Obstakel;
      
      if(Time_Of_Flight_Links > Koers_Value + Koers_Marge || Time_Of_Flight_Links < Koers_Value - Koers_Marge)
      Stap = Actie_Proces_Koers;
      break;
      
    case (Actie_Proces_Gewas):
      if(Signaal == Afgegeven);
      Stap = Rijden;
      break;
      
    case (Actie_Proces_Obstakel):
      if(Ultrasoon_Links_Voor > Arm_Lengte && Ultrasoon_Voor > Arm_Lengte && Ultrasoon_Rechts_Voor > Arm_Lengte);
      Stap = Rijden;
      break;     
      
    case (Actie_Proces_Koers):
      if(Time_Of_Flight_Links > Koers_Value - Koers_Marge && Time_Of_Flight_Links < Koers_Value + Koers_Marge); //valt Binnen de Marges van de koers
      Stap = Rijden; 
      if(Time_Of_Flight_Rechts > Koers_Value - Koers_Marge && Time_Of_Flight_Rechst < Koers_Value + Koers_Marge); //valt Binnen de Marges van de koers
      Stap = Rijden; 
      break;
      
    case (Volg_Modus):
      break;
  }
}
