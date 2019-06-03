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
#define Rpin_ain2  26
#define Rpin_ain1  27
#define Rpin_bin1  28
#define Rpin_bin2  29

#define Motor_Speed_Max     120                 //100rpm is max reacheble speed op 5V
#define Motor_Speed_Follow  (Motor_speed_max/7) //70% speed for following person
#define Motor_Speed_Half    (Motor_speed_max/2)
#define Motor_Speed_Stop    0                   //Set motor stil but is still running
#define Aantal_Rond         2*STEPS
#define Aantal_Rond         3*STEPS

#define Volg_Modus            -1
#define Idle                  0
#define Rijden                1
#define Actie_Proces_Gewas    2
#define Actie_Proces_Obstakel 3
#define Actie_Proces_Koers    4

#define Volgmodus -1
#define Autonoom  1

int Koers = 0;
#define Rechts_Afslaan 1
#define Links_Afslaan -1

#define Rij_Snelheid    512
#define Arm_Lengte      87,5 //milimeter 1/8 Schaalmodel van 0,7 Meter
#define Gewas_Afstand   50 //afstand van de gewassen tot de agv (misschien iets meer)
#define Koers_Value     10 //afstand van de rand van de gewassen tot agv
#define Koers_Marge     10 //hoeveelheid speelruimte van de koers van de agv

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

#define Time_Of_Flight_Links 20
#define Time_Of_Flight_Rechts 22
#define Signaal_Ledjes 23

Stepper Stepper_Links(STEPS, Lpin_ain2, Lpin_ain1, Lpin_bin1, Lpin_bin2);
Stepper Stepper_Rechts(STEPS, Rpin_ain2, Rpin_ain1, Rpin_bin1, Rpin_bin2);

void Actie_Proces_Gewas_Functie();
void Actie_Proces_Obstakel_Functie();
void Volg_Modus_Functie();
void Actie_Proces_Koers_Functie(Waarde voor bocht links of rechts);
int Distance_Cal(int trigPin, int echoPin);

int Stap = 0;
int Status = 0;
float Distance = 0;

void setup() 
{
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

  digitalWrite(Ultrasoon_Voor_Trigger, LOW);
  digitalWrite(Ultrasoon_Links_Voor_Trigger, LOW);
  digitalWrite(Ultrasoon_Rechts_Voor_Trigger, LOW);
  digitalWrite(Ultrasoon_Links_Achter_Trigger, LOW);
  digitalWrite(Ultrasoon_Rechts_Achter_Trigger, LOW);

  pinMode(Time_Of_Flight_Links, INPUT);//AANPASSEN? ToF links en rechts op zelfde pin met switches die omschakelen na het geheel uitvoeren van een bochtfunctie
  pinMode(Time_Of_Flight_Rechts, INPUT);

  pinMode(Signaal_Ledjes, OUTPUT);
}


void loop() {
  switch(Stap) //Switchcase met states
  {
    case (Idle): 
      //Motoren uit en wachten
        Stepper_Links.setSpeed(Motor_Speed_Stop);
        Stepper_Rechts.setSpeed(Motor_Speed_Stop);
      break;
      
    case (Rijden):
      //Steppers op standaard rijsnelheid
      Stepper_Links.setSpeed(Motor_Speed_Max);
      Stepper_Rechts.setSpeed(Motor_Speed_Max);
      break;
      
    case (Actie_Proces_Gewas):
      //Stappenmotoren stilzetten en signaal afgeven
      Actie_Proces_Gewas_Functie();
      break;
      
    case (Actie_Proces_Obstakel):
      //Gepaste Afstand houden --> Stilstaan of langzamer gaan rijden.
      Actie_Proces_Koers_Functie();
      break; 
          
    case (Actie_Proces_Koers):
      //Bijsturen en Bochtnemen
      Actie_Proces_Koers_Functie();
      break;
      
    case (Volg_Modus):
      //Rijden, Stoppen en Bijsturen.
      Volg_Modus_Functie();
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
       
      Distance = Distance_Cal(Ultrasoon_Voor_Trigger, Ultrasoon_Voor_Echo);
      if(Distance < Arm_Lengte || Distance > 0)
      {
         Stap = Actie_Proces_Obstakel;
      } 

      Distance = Distance_Cal(Ultrasoon_Links_Achter_Trigger, Ultrasoon_Links_Achter_Echo);
      if(Distance < Gewas_Afstand || Distance > 0)
      {
        Stap = Actie_Proces_Gewas;
      }

      Distance = Distance_Cal(Ultrasoon_Rechts_Achter_Trigger, Ultrasoon_Rechts_Achter_Echo);
      if(Distance < Gewas_Afstand || Distance > 0)
      {
        Stap = Actie_Proces_Gewas; 
      }

      if(digitalRead(Time_Of_Flight_Links) > Koers_Value + Koers_Marge || digitalRead(Time_Of_Flight_Rechts) < Koers_Value - Koers_Marge)
        Stap = Actie_Proces_Koers;
      break;
      
    case (Actie_Proces_Gewas):
      Stap = Rijden;
      break;
      
    case (Actie_Proces_Obstakel):
      Distance = Distance_Cal(Ultrasoon_Voor_Trigger, Ultrasoon_Voor_Echo);
      if(Distance > Arm_Lengte)
      {
         Stap = Rijden;
      } 
      break;     
      
    case (Actie_Proces_Koers):
      if(digitalRead(Time_Of_Flight_Links) > Koers_Value - Koers_Marge && digitalRead(Time_Of_Flight_Links) < Koers_Value + Koers_Marge); //valt Binnen de Marges van de koers
        Stap = Rijden; 
      if(digitalRead(Time_Of_Flight_Rechts) > Koers_Value - Koers_Marge && digitalRead(Time_Of_Flight_Rechts) < Koers_Value + Koers_Marge); //valt Binnen de Marges van de koers
       Stap = Rijden; 
      break;
      
    case (Volg_Modus):
      break;
  }
}



void Actie_Proces_Gewas_Functie()
{
  Stepper_Links.setSpeed(Motor_Speed_Stop);
  Stepper_Rechts.setSpeed(Motor_Speed_Stop);
  delay(100);
  digitalWrite(Signaal_Ledjes, HIGH);
  delay(100);
  digitalWrite(Signaal_Ledjes, LOW);
}

void Actie_Proces_Obstakel_Functie()
{
  Stepper_Links.setSpeed(Motor_Speed_Stop);
  Stepper_Rechts.setSpeed(Motor_Speed_Stop);
}

void Actie_Proces_Koers_Functie(Waarde voor bocht links of rechts)
{
    //ToF sensoren uit
  if(bocht == rechts)                         //Bij een bocht naar links of rechts wordt de binnenste motor op 50% gezet
  {
    Stepper_Links.setSpeed(Motor_Speed_Half);
    Stepper_Rechts.setSpeed(Motor_Speed_Max);
    
    for(i = 0; i<Aantal_rond_bocht; i++)
    {
      Stepper_Links.step(2);      //Het linker wiel moet een grotere afstand af leggen. gekozen voor 3x zo groot
      Stepper_Rechts.step(1);
    }
  }

  if(bocht == links)
  {
    //Bochten ++
    Stepper_Links.setSpeed(Motor_Speed_Half);
    Stepper_Rechts.setSpeed(Motor_Speed_Max);
    for(i = 0; i<Aantal_rond_bocht; i++)
    {
    Stepper_Rechts.step(2);      //Het rechter wiel moet een grotere afstand af leggen. gekozen voor 3x zo groot
    Stepper_Links.step(1);
    }
  }
}

void Volg_Modus_Functie()
{
  Stepper_Links.setSpeed(Motor_speed_follow);
  Stepper_Rechts.setSpeed(Motor_speed_follow);
  //if statements met versnellen van de motor/vertragen moeten hier komen
}

int Distance_Cal(int trigPin, int echoPin)
{
  float duration;
  float distance;
  float gem_distance;
  for(int i = 0; i < 10; i++)
  {
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = duration* 0.34/2; 
    if(distance > 0)
    {
      gem_distance = gem_distance + distance;
    }
    else
    {
      i--;
    }
  }
  return gem_distance;
}
