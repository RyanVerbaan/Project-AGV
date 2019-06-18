#include "Defines.h"
#include <Stepper.h>


int Stap = 0;
float Distance = 0;
int teller = 0;
int Koers = 0;
int Bocht = 0;


Stepper Stepper_Links(STEPS, Pin_Links_Ain2, Pin_Links_Ain1, Pin_Links_Bin1, Pin_Links_Bin2);
Stepper Stepper_Rechts(STEPS, Pin_Rechts_Ain2, Pin_Rechts_Ain1, Pin_Rechts_Bin1, Pin_Rechts_Bin2);


//Hier worden alle functies geïnialiseerd
float Distance_Cal(int trigPin, int echoPin);
void Actie_Proces_Gewas_Functie();
void Actie_Proces_Obstakel_Functie();
void Volg_Modus_Functie();
void Actie_Proces_Koers_Functie(int Bocht);

void setup()
{
  Serial.begin(9600);
  Serial.println("~~~~~~~~~~~~~~Setup~~~~~~~~~~~~~~~~");
  //pinModes Ultrasoon
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
  //Overige Pinmodes
  pinMode(Signaal_Ledjes, OUTPUT);
  
  //Digital Writes
  digitalWrite(Ultrasoon_Voor_Trigger, LOW);
  digitalWrite(Ultrasoon_Links_Voor_Trigger, LOW);
  digitalWrite(Ultrasoon_Rechts_Voor_Trigger, LOW);
  digitalWrite(Ultrasoon_Links_Achter_Trigger, LOW);
  digitalWrite(Ultrasoon_Rechts_Achter_Trigger, LOW);
}

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+loop-+-+-+-+-+-+-+--+-+-+-+-+-+-+-++-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

void loop()
{
  //In de eerste switch case worden de states uitgevoerd
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++Switch toestanden+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
  switch(Stap)
  {
    case (Idle): 
      //Motoren worden stilgezet en wachten op een volgende activiteit
        Stepper_Links.step(0);
        Stepper_Links.step(0);
        Serial.println("Idle State");
      break;
      
    case (Rijden):
      //Default state de steppers rijden vooruit
      Stepper_Rechts.step(1);
      Stepper_Links.step(1); 
      Serial.println("Rijden State");
      break;
      
    case (Actie_Proces_Gewas):
      //Stappenmotoren stilzetten en signaal afgeven
      Actie_Proces_Gewas_Functie();
      Serial.println("Gewas State");
      break;
      
    case (Actie_Proces_Obstakel):
      //Hier worden bomen gedetecteerd aan de zijkant met de ultrasone sensoren
      Actie_Proces_Obstakel_Functie();
      Serial.println("Obstakel State");
      break; 
          
    case (Actie_Proces_Koers):
      //Bijsturen en Bochtnemen
      Actie_Proces_Koers_Functie(Bocht);
      Serial.println("Koers State");
      break;
      
    case (Volg_Modus):
      //Rijden, Stoppen en Bijsturen.
      Serial.println("Volg State");
      Volg_Modus_Functie();
      break;
  }
  //In de tweede stwitch case worden de overgangen uitgevoerd
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++Switch overgangen+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
  switch(Stap)
  {
    case (Idle):
      if(digitalRead(Volgmodus_Autonoom_Knop) == LOW)
      {
        Stap = Rijden;
      }
      if(digitalRead(Volgmodus_Autonoom_Knop) == HIGH)
      {
        Stap = Volg_Modus;
      }
      Serial.println("Idle overgang");
      break;
/*------------------------------------------------------------case Rijden overgang-------------------------------------------------------------*/      
    case (Rijden):
      Serial.println("Rijden Overgang");
      if(digitalRead(Volgmodus_Autonoom_Knop) == HIGH)
      {
       Stap = Idle;
      }
/*~~~~~~~~~~~~~~~~~ultrasoon checks~~~~~~~~~~~~~~~~~~~~~~~~~*/      
      Distance = Distance_Cal(Ultrasoon_Voor_Trigger, Ultrasoon_Voor_Echo);                   //Ultrasoon voor kijken
      if(Distance < Arm_Lengte && Distance > 0)
      {
         Serial.print("Ultrasoon voor "); Serial.println(Distance);
         Stap = Actie_Proces_Obstakel;
      }
      
      Distance = Distance_Cal(Ultrasoon_Links_Achter_Trigger, Ultrasoon_Links_Achter_Echo);   //Ultrasoon Links kijken
      if(Distance < Gewas_Afstand && Distance > 0)
      {
        Serial.print("Ultrasoon lachter "); Serial.println(Distance);
//        Stap = Actie_Proces_Gewas; // Deze sensor heeft momenteel issues verder werkt de code zoals gewild zonder ToFs
      }
      
      Distance = Distance_Cal(Ultrasoon_Rechts_Achter_Trigger, Ultrasoon_Rechts_Achter_Echo); //Ultrasoon Rechts kijken
      if(Distance < Gewas_Afstand && Distance > 0)
      {
        Serial.print("Ultrasoon rachter "); Serial.println(Distance);
        Stap = Actie_Proces_Gewas; 
      }
      break;

/*------------------------------------------------------------case Actie_Proces_Gewas overgang-------------------------------------------------------------*/
    case (Actie_Proces_Gewas):
      Serial.println("Gewas overgang");
      Stap = Rijden;
      break;
/*------------------------------------------------------------case Actie_Proces_Gewas overgang-------------------------------------------------------------*/
    case (Actie_Proces_Obstakel):
      Serial.println("Obstakel overgang");
      Distance = Distance_Cal(Ultrasoon_Voor_Trigger, Ultrasoon_Voor_Echo);
      if(Distance > Arm_Lengte)
      {
         Stap = Rijden;
      } 
      break;     
/*------------------------------------------------------------case Actie_Proces_Koers overgang-------------------------------------------------------------*/     
    case (Actie_Proces_Koers):

      break;  
/*------------------------------------------------------------case Volg_Modus overgang-------------------------------------------------------------*/   
    case (Volg_Modus):
      Serial.println("Volg Overgang");
      Stepper_Links.step(1);
      Stepper_Rechts.step(1);
      if(digitalRead(Volgmodus_Autonoom_Knop) == LOW)
      {
        Stap = Idle;
      }
      break;
    default:
      break;
  }
}// dit is het einde van de loop (laatste accolade)

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+/loop/-+-+-+-+-+-+-+--+-+-+-+-+-+-+-++-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

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

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+Actie_Proces_Koers_Functie-+-+-+-+-+-+-+--+-+-+-+-+-+-+-++-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

void Actie_Proces_Koers_Functie(int Bocht)
{
    switch(Bocht)
    {
      case(Rechtsom):
        Stepper_Links.step(2);
        Stepper_Rechts.step(1);
        break;
        
      case(Linksom):
        Stepper_Rechts.step(2);
        Stepper_Links.step(1);
        break;
    }  
}

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+Actie_Proces_Obstakel_Functie-+-+-+-+-+-+-+--+-+-+-+-+-+-+-++-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

void Actie_Proces_Obstakel_Functie()
{
      Stepper_Links.step(0);
      Stepper_Rechts.step(0);
}

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+Actie_Proces_Gewas_Functie-+-+-+-+-+-+-+--+-+-+-+-+-+-+-++-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

void Actie_Proces_Gewas_Functie()
{
  Stepper_Links.step(0);
  Stepper_Rechts.step(0);
  delay(500);
  digitalWrite(Signaal_Ledjes, HIGH);
  delay(500);
  digitalWrite(Signaal_Ledjes, LOW);
}

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+Volg_Modus_Functie-+-+-+-+-+-+-+--+-+-+-+-+-+-+-++-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

void Volg_Modus_Functie()
{
  
  Distance = Distance_Cal(Ultrasoon_Voor_Trigger, Ultrasoon_Voor_Echo);
  if(Distance < (Arm_Lengte*0.8))
    {
      Stepper_Links.step(0);
      Stepper_Rechts.step(0);
    }
  if(Distance > (Arm_Lengte*1.2))
  {
      Stepper_Links.step(1);
      Stepper_Rechts.step(1);
  }
}//volg_modus_functie
