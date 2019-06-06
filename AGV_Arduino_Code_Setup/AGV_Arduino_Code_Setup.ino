#include <Stepper.h>
#include <Wire.h>
#include <VL6180X.h>

//Stap Defines
#define Volg_Modus            -1
#define Idle                  0
#define Rijden                1
#define Actie_Proces_Gewas    2
#define Actie_Proces_Obstakel 3
#define Actie_Proces_Koers    4

#define Volgmodus             -1
#define Autonoom              1

//Pin Defines
//Pinnen voor linker stepper
#define Lpin_ain2  28
#define Lpin_ain1  29
#define Lpin_bin1  30
#define Lpin_bin2  31
//Pinnen voor retcher stepper
#define Rpin_ain2  36
#define Rpin_ain1  37
#define Rpin_bin1  38
#define Rpin_bin2  39
//Ultrasoon Pinnen Trigger
#define Ultrasoon_Voor_Trigger          12
#define Ultrasoon_Links_Voor_Trigger    10
#define Ultrasoon_Rechts_Voor_Trigger   8
#define Ultrasoon_Links_Achter_Trigger  6
#define Ultrasoon_Rechts_Achter_Trigger 4
//Ultrasoon Pinnen Echo
#define Ultrasoon_Voor_Echo             11
#define Ultrasoon_Links_Voor_Echo       9
#define Ultrasoon_Rechts_Voor_Echo      7
#define Ultrasoon_Links_Achter_Echo     5
#define Ultrasoon_Rechts_Achter_Echo    3
//Overige Pinnen
#define SDA_Pin                         20
#define SCL_Pin                         21
#define Shut_ToF_Rechts                 11
#define Shut_ToF_Links                  10
#define Signaal_Ledjes                  23

//Time of Flight Adressen
#define address0 0x20
#define address1 0x22

//Constanten Defines en Variabelen
#define STEPS     200
#define Motor_Speed_Max       120                 
#define Motor_Speed_Stop      0    

#define RANGE 1
int Koers = 0;
#define Rechts_Afslaan        1
#define Links_Afslaan         -1

#define Rij_Snelheid          512
#define Arm_Lengte            87,5  //milimeter 1/8 Schaalmodel van 0,7 Meter
#define Gewas_Afstand         50    //afstand van de gewassen tot de agv (misschien iets meer)
#define Koers_Value           10    //afstand van de rand van de gewassen tot agv
#define Koers_Marge           10    //hoeveelheid speelruimte van de koers van de agv
#define Bocht                 0
#define Linksom               -1
#define Rechtsom              1

void Actie_Proces_Gewas_Functie();
void Actie_Proces_Obstakel_Functie();
void Volg_Modus_Functie();
void Actie_Proces_Koers_Functie(int Obstakel);
int Distance_Cal(int trigPin, int echoPin);

int Bocht = Bocht_Links;
int Stap = 0;
int Status = 0;
float Distance = 0;
int Obstakel_Waarde = 0;

void setup() 
{
  Wire.begin();
  Serial.begin();
  
  //Stepper Settings
  Stepper Stepper_Links(STEPS, Lpin_ain2, Lpin_ain1, Lpin_bin1, Lpin_bin2);
  Stepper Stepper_Rechts(STEPS, Rpin_ain2, Rpin_ain1, Rpin_bin1, Rpin_bin2);

  //ToF Instance
  VL6180X ToF_Rechts;
  VL6180X ToF_Links;


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
  //Pinmodes Time of Flight
  pinMode(Time_Of_Flight_Links, INPUT);
  pinMode(Time_Of_Flight_Rechts, INPUT);
  //Overige Pinmodes
  pinMode(Signaal_Ledjes, OUTPUT);
  pinMode(Shut_ToF_Rechts,OUTPUT);
  pinMode(Shut_ToF_Links,OUTPUT);

  //Digital Writes
  digitalWrite(Ultrasoon_Voor_Trigger, LOW);
  digitalWrite(Ultrasoon_Links_Voor_Trigger, LOW);
  digitalWrite(Ultrasoon_Rechts_Voor_Trigger, LOW);
  digitalWrite(Ultrasoon_Links_Achter_Trigger, LOW);
  digitalWrite(Ultrasoon_Rechts_Achter_Trigger, LOW);

  // ToF Rechts
  digitalWrite(enablePin0, HIGH);
  delay(50);
  ToF_Rechts.init();
  ToF_Rechts.configureDefault();
  ToF_Rechts.setAddress(address0);
  ToF_Rechts.writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 30);
  ToF_Rechts.writeReg16Bit(VL6180X::SYSALS__INTEGRATION_PERIOD, 50);
  ToF_Rechts.setTimeout(500);
  ToF_Rechts.stopContinuous();
  ToF_Rechts.setScaling(RANGE); // configure range or precision 1, 2 oder 3 mm
  delay(300);
  ToF_Rechts.startInterleavedContinuous(100);
  delay(100);

  // ToF Links
  digitalWrite(enablePin1, HIGH);
  delay(50);
  ToF_Links.init();
  ToF_Links.configureDefault();
  ToF_Links.setAddress(address1);
  ToF_Links.writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 30);
  ToF_Links.writeReg16Bit(VL6180X::SYSALS__INTEGRATION_PERIOD, 50);
  ToF_Links.setTimeout(500);
  ToF_Links.stopContinuous();
  ToF_Links.setScaling(RANGE);
  delay(300);
  ToF_Links.startInterleavedContinuous(100);
  delay(100);
}


void loop()
{
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
      Actie_Proces_Obstakel_Functie();
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
      Stepper_Links.step(1);
      Stepper_Rechts.step(1);
      if(Status != Autonoom)
      {
       Stap = Idle;
      }
      Distance = Distance_Cal(Ultrasoon_Voor_Trigger, Ultrasoon_Voor_Echo);                   //Ultrasoon voor kijken
      if(Distance < Arm_Lengte && Distance > 0)
      {
         Stap = Volg_Modus;
      }
      Distance = Distance_Cal(Ultrasoon_Links_Achter_Trigger, Ultrasoon_Links_Achter_Echo);   //Ultrasoon Links kijken
      if(Distance < Gewas_Afstand && Distance > 0)
      {
        Obstakel_Waarde = Obstakel_Boom;
        Stap = Actie_Proces_Gewas;
      }
      Distance = Distance_Cal(Ultrasoon_Rechts_Achter_Trigger, Ultrasoon_Rechts_Achter_Echo); //Ultrasoon Rechts kijken
      if(Distance < Gewas_Afstand && Distance > 0)
      {
        Obstakel_Waarde = Obstakel_Boom;
        Stap = Actie_Proces_Gewas; 
      }
      if(ToF_Rechts.readRangeContinuousMillimeters()) > Koers_Value + Koers_Marge || ToF_Rechts.readRangeContinuousMillimeters() < Koers_Value - Koers_Marge)
      {
        Bocht = Linksom
        Stap = Actie_Proces_Koers;
      }
      if(ToF_Rechts.readRangeContinuousMillimeters()) > Koers_Value + Koers_Marge)
      {
        Bocht = Rechtsom;
        Stap = Actie_Proces_Koers;
      }
      if(ToF_Links.readRangeContinuousMillimeters()) > Koers_Value + Koers_Marge)
      {
        Bocht = Linksom;
        Stap = Actie_Proces_Koers;
      }
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
  delay(500);
  digitalWrite(Signaal_Ledjes, HIGH);
  delay(500);
  digitalWrite(Signaal_Ledjes, LOW);
}

void Actie_Proces_Obstakel_Functie()
{
  Stepper_Links.setSpeed(Motor_Speed_Stop);
  Stepper_Rechts.setSpeed(Motor_Speed_Stop);
}

void Actie_Proces_Koers_Functie(int Obstakel)
{
    switch(Obstakel)
    {
      case(Obstakel_Bocht_Rechts):
        for(int i = 0; i < 5; i++)  // klein stukkie voor uit
        {
          Stepper_Links.step(1);      
          Stepper_Rechts.step(1);
        }
        for(int i = 0; i < 10; i++) // 180 graden draaien
        {
          Stepper_Links.step(1);      
          Stepper_Rechts.step(1);
          Stepper_Links.step(1);
        }
        for(int i = 0; i < 5; i++)  // klein stukkie voor uit
        {
          Stepper_Links.step(1);      
          Stepper_Rechts.step(1);
        }
        break;
      case(Obstakel_Bocht_Links):
        for(int i = 0; i < 5; i++)  // klein stukkie voor uit
        {
          Stepper_Links.step(1);      
          Stepper_Rechts.step(1);
        }
        for(int i = 0; i < 10; i++) // 180 graden draaien
        {
          Stepper_Rechts.step(1);
          Stepper_Links.step(1);      
          Stepper_Rechts.step(1);
        }
        for(int i = 0; i < 5; i++)  // klein stukkie voor uit
        {
          Stepper_Links.step(1);      
          Stepper_Rechts.step(1);
        }
        Bocht = Bocht_Rechts;
        break;
    }  
}

void Volg_Modus_Functie()
{
  
  Distance = Distance_Cal(Ultrasoon_Voor_Trigger, Ultrasoon_Voor_Echo);
  if(Distance < (Arm_Lengte*0.8))
    {
      delay(500);
    }
  if(Distance > (Arm_Lengte*1.2))
  {
    Stepper_Links.step(1);
    Stepper_Rechts.step(1);
  }
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
    duration = pulseIn(echoPin, HIGH, 3000);
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
