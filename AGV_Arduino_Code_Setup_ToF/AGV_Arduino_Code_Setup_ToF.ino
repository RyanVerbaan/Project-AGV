#include <Wire.h>
#include "Adafruit_VL6180X.h"

Adafruit_VL6180X vl = Adafruit_VL6180X();

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

void setup() 
{
  Serial.begin(115200);
  pinMode(Stepper_Links_Pin, OUTPUT);
  pinMode(Stepper_Rechts_Pin, OUTPUT);
  
  pinMode(Ultrasoon_Voor, INPUT);
  pinMode(Ultrasoon_Links_Voor, INPUT);
  pinMode(Ultrasoon_Rechts_Voor, INPUT);
  pinMode(Ultrasoon_Links_Achter, INPUT);
  pinMode(Ultrasoon_Rechts_Achter, INPUT);
  
  pinMode(Time_Of_Flight_Links, OUTPUT);//Dit zijn de transistoren die bepalen welk van de ToF sensoren is aangesloten
  pinMode(Time_Of_Flight_Rechts, OUTPUT);
  
  pinMode(Signaal_Ledjes, OUTPUT);
  
  digitalWrite(Stepper_Links_Pin, LOW);
  digitalWrite(Stepper_Rechts_Pin, LOW);

  pinMode(Time_Of_Flight_Links, HIGH); //Deze waardes mogen nooit tegelijkertijd HIGH zijn.
  pinMode(Time_Of_Flight_Rechts, LOW);
}//setup

Actie_Proces_Gewas()
{
  digitalWrite(Stepper_Links_Pin, LOW);
  digitalWrite(Stepper_Rechts_Pin, LOW);
  delay(100);
  digitalWrite(Signaal_Ledjes, HIGH);
  delay(100);
  digitalWrite(Signaal_Ledjes, LOW);
}//Actie_Proces_Gewas

Actie_Proces_Obstakel()
{
  digitalWrite(Stepper_Links_Pin, LOW);
  digitalWrite(Stepper_Rechts_Pin, LOW);
}//Actie_Proces_Obstakel

Bocht_Links()
{ //hoeveelheid_bochten++;
  digitalWrite(Stepper_Links_Pin, Lage_Bocht_Snelheid);
  digitalWrite(Stepper_Rechts_Pin, Hoge_Bocht_Snelheid);
}//Bocht_Links

Bocht_Rechts()
{ //ToF_switch, LOW)
  digitalWrite(Stepper_Links_Pin, Hoge_Bocht_Snelheid);
  digitalWrite(Stepper_Rechts_Pin, Lage_Bocht_Snelheid);  
}//Bocht_Rechts

Volg_Modus()
{
  
}//Volg_Modus

float ToF_meting()
{
  int randvoorwaarde_errors = 0;
  float totale_waarde = 0;
  float gemiddelde = 0;
  for(int i =0; i < 10; i++){
    
    uint8_t range = vl.readRange();
    uint8_t status = vl.readRangeStatus();
    if (status != VL6180X_ERROR_NONE) {
      i--;
      randvoorwaarde_errors++;
      if(randvoorwaarde_errors == 5);
        return 0;
    }
    else{
      totale_waarde += range;
    }
  }//for
  
  gemiddelde = totale_waarde/10;
  return (1.09395*(gemiddelde-5.5)/11+1.399406);//pas hier de gevonden functie toe uit de proef
}//ToF_meting

void loop() {
  switch(Stap) //Switchcase met states
  {
    case (Idle): 
      //Motoren uit en wachten
      digitalWrite(Stepper_Links_Pin, LOW); 
      digitalWrite(Stepper_Rechts_Pin, LOW);
      break;
      
    case (Rijden):
      //Steppers op standaard rijsnelheid
      digitalWrite(Stepper_Links_Pin, Rij_Snelheid);
      digitalWrite(Stepper_Rechts_Pin, Rij_Snelheid);
      break;
      
    case (Actie_Proces_Gewas):
      //Stappenmotoren stilzetten en signaal afgeven
      Actie_Proces_Gewas();
      break;
      
    case (Actie_Proces_Obstakel):
      //Gepaste Afstand houden --> Stilstaan of langzamer gaan rijden.
      if(Koers == Links_Afslaan)
        Bocht_Links();
      if(Koers == Rechts_Afslaan)
        Bocht_Rechts();
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
      float afstand_tot_rand = ToF_meting();
      
      if(Status != Autonoom)
        Stap = Idle;

      if(Ultrasoon_Links_Achter == HIGH || Ultrasoon_Rechts_Achter == HIGH); //moet nog vergeleken met een waarde maar niet op HIGH
        Stap = Actie_Proces_Gewas;
      
      if(Ultrasoon_Links_Voor == HIGH || Ultrasoon_Voor == HIGH || Ultrasoon_Rechts_Voor == HIGH); //moet nog vergeleken met een waarde maar niet op HIGH
        Stap = Actie_Proces_Obstakel;

      if(afstand_tot_rand != 0 && afstand_tot_rand > Koers_Value + Koers_Marge)
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
      float afstand_tot_rand = ToF_meting();
      if(afstand_tot_rand != 0 && afstand_tot_rand < Koers_Value + Koers_Marge); //valt Binnen de Marges van de koers
        Stap = Rijden; 
      break;
      
    case (Volg_Modus):
      break;
  }
}
