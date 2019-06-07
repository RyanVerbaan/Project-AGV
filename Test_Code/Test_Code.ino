#include "Defines.h"
#include <Stepper.h>
#include <Wire.h> 
#include <VL6180X.h>

int Stap = 0;
float Distance = 0;
int teller = 0;
int Status = 0;
int Koers = 0;
int Bocht = 0;

//Stepper Stepper_Links(STEPS, Pin_Links_Ain2, Pin_Links_Ain1, Pin_Links_Bin1, Pin_Links_Bin2);
//Stepper Stepper_Rechts(STEPS, Pin_Rechts_Ain2, Pin_Rechts_Ain1, Pin_Rechts_Bin1, Pin_Rechts_Bin2);

Stepper Stepper_Links(STEPS, Pin_Links_Ain2, Pin_Links_Ain1, Pin_Links_Bin1, Pin_Links_Bin2);
Stepper Stepper_Rechts(STEPS, Pin_Rechts_Ain2, Pin_Rechts_Ain1, Pin_Rechts_Bin1, Pin_Rechts_Bin2);


//Hier worden alle functies geïnialiseerd
float Distance_Cal(int trigPin, int echoPin);
void Actie_Proces_Gewas_Functie();
void Actie_Proces_Obstakel_Functie();
void Volg_Modus_Functie();
void Actie_Proces_Koers_Functie(int Bocht);
void Init_Pins();
 //ToF Instance
  VL6180X ToF_Rechts;
  VL6180X ToF_Links;


void setup()
{
  Wire.begin();
  Serial.begin(9600);

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
  pinMode(Shut_ToF_Rechts,OUTPUT);
  pinMode(Shut_ToF_Links,OUTPUT);

  //Digital Writes
  digitalWrite(Ultrasoon_Voor_Trigger, LOW);
  digitalWrite(Ultrasoon_Links_Voor_Trigger, LOW);
  digitalWrite(Ultrasoon_Rechts_Voor_Trigger, LOW);
  digitalWrite(Ultrasoon_Links_Achter_Trigger, LOW);
  digitalWrite(Ultrasoon_Rechts_Achter_Trigger, LOW);


  // ToF Rechts
  digitalWrite(Shut_ToF_Rechts, HIGH);
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
  digitalWrite(Shut_ToF_Links, HIGH);
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
  //Init_Pins();
  delay(1000); //Een delay voordat de AGV Begint met het rijden in de boomgaard

}

void loop()
{
  //In de eerste switch case worden de states uitgevoerd
  switch(Stap)
  {
    case (Idle): 
      //Motoren worden stilgezet en wachten op een volgende activiteit
      Serial.println("Idle State");
      Stepper_Rechts.step(0);
      Stepper_Links.step(0);
      break;
      
    case (Rijden):
      //Default state de steppers rijden vooruit
      Serial.println("Rijden State");
      Stepper_Rechts.step(2);
      Stepper_Links.step(2);
      break;
      
    case (Actie_Proces_Gewas):
      //Stappenmotoren stilzetten en signaal afgeven
      Serial.println("Gewas State");
      Actie_Proces_Gewas_Functie();
      break;
      
    case (Actie_Proces_Obstakel):
      //Hier worden bomen gedetecteerd aan de zijkant met de ultrasone sensoren
      Serial.println("Obstakel State");
      Actie_Proces_Obstakel_Functie();
      break; 
          
    case (Actie_Proces_Koers):
      //Bijsturen en Bochtnemen
      Serial.println("Koers State");
      Actie_Proces_Koers_Functie(Bocht);
      break;
      
    case (Volg_Modus):
      //Rijden, Stoppen en Bijsturen.
      Serial.println("Volg State");
      Volg_Modus_Functie();
      break;
    default:
      break;
  }
  
  //In de tweede stwitch case worden de overgangen uitgevoerd
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
      
    case (Rijden):
      Serial.println("Rijden Overgang");
      if(digitalRead(Volgmodus_Autonoom_Knop) == HIGH)
      {
       Stap = Idle;
      }
      
      Distance = Distance_Cal(Ultrasoon_Voor_Trigger, Ultrasoon_Voor_Echo);                   //Ultrasoon voor kijken
      Serial.println(Distance);
      if(Distance < Arm_Lengte && Distance > 0)
      {
         Stap = Actie_Proces_Obstakel;
      }
      
      Distance = Distance_Cal(Ultrasoon_Links_Achter_Trigger, Ultrasoon_Links_Achter_Echo);   //Ultrasoon Links kijken
      Serial.println(Distance);
      if(Distance < Gewas_Afstand && Distance > 0)
      {
        Stap = Actie_Proces_Gewas;
      }
      
      Distance = Distance_Cal(Ultrasoon_Rechts_Achter_Trigger, Ultrasoon_Rechts_Achter_Echo); //Ultrasoon Rechts kijken
      Serial.println(Distance);
      if(Distance < Gewas_Afstand && Distance > 0)
      {
        Stap = Actie_Proces_Gewas; 
      }

      //Serial.println(ToF_Rechts.readRangeContinuousMillimeters());
      if(ToF_Rechts.readRangeContinuousMillimeters() > Koers_Value + Koers_Marge)
      {
        Bocht = Rechtsom;
        Stap = Actie_Proces_Koers;
      }

      //Serial.println(ToF_Links.readRangeContinuousMillimeters());
      if(ToF_Links.readRangeContinuousMillimeters() > Koers_Value + Koers_Marge)
      {
        Bocht = Linksom;
        Stap = Actie_Proces_Koers;
      }
      break;
    
    case (Actie_Proces_Gewas):
      Serial.println("Gewas overgang");
      Stap = Rijden;
      break;
 
    case (Actie_Proces_Obstakel):
      Serial.println("Obstakel overgang");
      Distance = Distance_Cal(Ultrasoon_Voor_Trigger, Ultrasoon_Voor_Echo);
      if(Distance > Arm_Lengte)
      {
         Stap = Rijden;
      } 
      break;     
     
    case (Actie_Proces_Koers):
      Serial.println("Koers Overgang");
      Serial.println(ToF_Rechts.readRangeContinuousMillimeters());
      if((ToF_Rechts.readRangeContinuousMillimeters() > Koers_Value - Koers_Marge) && ((ToF_Rechts.readRangeContinuousMillimeters() < Koers_Value + Koers_Marge))) //valt Binnen de Marges van de koers
      {
        Stap = Rijden;
      } 
      Serial.println(ToF_Links.readRangeContinuousMillimeters());
      if((ToF_Links.readRangeContinuousMillimeters() > Koers_Value - Koers_Marge) && ((ToF_Links.readRangeContinuousMillimeters() < Koers_Value + Koers_Marge))) //valt Binnen de Marges van de koers
      {
        Stap = Rijden;
      } 
      break;     
    case (Volg_Modus):
      Serial.println("Volg Overgang");
      if(digitalRead(Volgmodus_Autonoom_Knop) == LOW)
      {
        Stap = Idle;
      }
      break;
    default:
      break;
  }
}// dit is het einde van de void loop (laatste accolade)

/*===========================================================================================================*/
/*===========================================================================================================*/

//hier onder worden alle functies gedefineerd

//void Init_Pins()
//{
//  Wire.begin();
//  Serial.begin();
//
//  //ToF Instance
//  VL6180X ToF_Rechts;
//  VL6180X ToF_Links;
//  
//  //pinModes Ultrasoon
//  pinMode(Ultrasoon_Voor_Trigger, OUTPUT);
//  pinMode(Ultrasoon_Voor_Echo, INPUT);
//  pinMode(Ultrasoon_Links_Voor_Trigger, OUTPUT);
//  pinMode(Ultrasoon_Links_Voor_Echo, INPUT);
//  pinMode(Ultrasoon_Rechts_Voor_Trigger, OUTPUT);
//  pinMode(Ultrasoon_Rechts_Voor_Echo, INPUT);
//  pinMode(Ultrasoon_Links_Achter_Trigger, OUTPUT);
//  pinMode(Ultrasoon_Links_Achter_Echo, INPUT);
//  pinMode(Ultrasoon_Rechts_Achter_Trigger, OUTPUT);
//  pinMode(Ultrasoon_Rechts_Achter_Echo, INPUT);
//  //Pinmodes Time of Flight
//  pinMode(Time_Of_Flight_Links, INPUT);
//  pinMode(Time_Of_Flight_Rechts, INPUT);
//  //Overige Pinmodes
//  pinMode(Signaal_Ledjes, OUTPUT);
//  pinMode(Shut_ToF_Rechts,OUTPUT);
//  pinMode(Shut_ToF_Links,OUTPUT);
//
//  //Digital Writes
//  digitalWrite(Ultrasoon_Voor_Trigger, LOW);
//  digitalWrite(Ultrasoon_Links_Voor_Trigger, LOW);
//  digitalWrite(Ultrasoon_Rechts_Voor_Trigger, LOW);
//  digitalWrite(Ultrasoon_Links_Achter_Trigger, LOW);
//  digitalWrite(Ultrasoon_Rechts_Achter_Trigger, LOW);
//
//  // ToF Rechts
//  digitalWrite(enablePin0, HIGH);
//  delay(50);
//  ToF_Rechts.init();
//  ToF_Rechts.configureDefault();
//  ToF_Rechts.setAddress(address0);
//  ToF_Rechts.writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 30);
//  ToF_Rechts.writeReg16Bit(VL6180X::SYSALS__INTEGRATION_PERIOD, 50);
//  ToF_Rechts.setTimeout(500);
//  ToF_Rechts.stopContinuous();
//  ToF_Rechts.setScaling(RANGE); // configure range or precision 1, 2 oder 3 mm
//  delay(300);
//  ToF_Rechts.startInterleavedContinuous(100);
//  delay(100);
//
//  // ToF Links
//  digitalWrite(enablePin1, HIGH);
//  delay(50);
//  ToF_Links.init();
//  ToF_Links.configureDefault();
//  ToF_Links.setAddress(address1);
//  ToF_Links.writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 30);
//  ToF_Links.writeReg16Bit(VL6180X::SYSALS__INTEGRATION_PERIOD, 50);
//  ToF_Links.setTimeout(500);
//  ToF_Links.stopContinuous();
//  ToF_Links.setScaling(RANGE);
//  delay(300);
//  ToF_Links.startInterleavedContinuous(100);
//  delay(100);
//}

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

void Actie_Proces_Koers_Functie(int Bocht)
{
    switch(Bocht)
    {
      case(Rechtsom):
        Stepper_Links.step(2);
        Stepper_Rechts.step(1);
        
//        for(int i = 0; i < 5; i++)  // klein stukkie voor uit
//        {
//          Stepper_Links.step(1);      
//          Stepper_Rechts.step(1);
//        }
//        for(int i = 0; i < 10; i++) // 180 graden draaien
//        {
//          Stepper_Links.step(1);      
//          Stepper_Rechts.step(1);
//          Stepper_Links.step(1);
//        }
//        for(int i = 0; i < 5; i++)  // klein stukkie voor uit
//        {
//          Stepper_Links.step(1);      
//          Stepper_Rechts.step(1);
//        }
        break;
        
      case(Linksom):
        Stepper_Rechts.step(2);
        Stepper_Links.step(1);
//        for(int i = 0; i < 5; i++)  // klein stukkie voor uit
//        {
//          Stepper_Links.step(1);      
//          Stepper_Rechts.step(1);
//        }
//        for(int i = 0; i < 10; i++) // 180 graden draaien
//        {
//          Stepper_Rechts.step(1);
//          Stepper_Links.step(1);      
//          Stepper_Rechts.step(1);
//        }
//        for(int i = 0; i < 5; i++)  // klein stukkie voor uit
//        {
//          Stepper_Links.step(1);      
//          Stepper_Rechts.step(1);
//        }
//        Bocht = Rechtsom;
        break;
    }  
}

void Actie_Proces_Obstakel_Functie()
{
  Stepper_Links.step(0);
  Stepper_Rechts.step(0);
}

void Actie_Proces_Gewas_Functie()
{
  Stepper_Links.step(Motor_Speed_Stop);
  Stepper_Rechts.step(Motor_Speed_Stop);
  delay(500);
  digitalWrite(Signaal_Ledjes, HIGH);
  delay(500);
  digitalWrite(Signaal_Ledjes, LOW);
}

void Volg_Modus_Functie()
{
  
  Distance = Distance_Cal(Ultrasoon_Voor_Trigger, Ultrasoon_Voor_Echo);
  if(Distance > Arm_Lengte)
    {
      Stepper_Links.step(1);
      Stepper_Rechts.step(1);
    }
  if(Distance < Arm_Lengte)
  {
      Stepper_Links.step(0);
      Stepper_Rechts.step(0);
  }
}
