#include "Defines.h"
#include <Stepper.h>

int Linksom  = 2;
int Rechtsom = 1;

int Stap = 0;
float Distance = 0;
int Koers = 0;
int Bocht = -1;
int Bochten_Patroon[] = {Linksom, Rechtsom, Linksom, Linksom, Rechtsom, Linksom};

int Distance_Voor_Rechts = 0;  
int Distance_Voor_Midden = 0;
int Distance_Voor_Links = 0;

int Correctie_Links = 0;
int Correctie_Rechts = 0;

Stepper Stepper_Links(STEPS, Pin_Links_Ain2, Pin_Links_Ain1, Pin_Links_Bin1, Pin_Links_Bin2);
Stepper Stepper_Rechts(STEPS, Pin_Rechts_Ain2, Pin_Rechts_Ain1, Pin_Rechts_Bin1, Pin_Rechts_Bin2);


float Distance_Cal(int trigPin, int echoPin);
void Bocht_linksom();
void Bocht_rechtsom();
void Klein_Stukje_Rijden();

void setup()
{
  Serial.begin(9600);
  Stepper_Links.setSpeed(40);
  Stepper_Rechts.setSpeed(40);
  
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
  
  pinMode(Standby_Pin_Rechts, OUTPUT);
  pinMode(Standby_Pin_Links, OUTPUT);
  
  pinMode(Eindstand_Pin_Links_Klein, INPUT_PULLUP);
  pinMode(Eindstand_Pin_Rechts_Klein, INPUT_PULLUP);

  pinMode(Eindstand_Pin_Links_Groot, INPUT_PULLUP);
  pinMode(Eindstand_Pin_Rechts_Groot, INPUT_PULLUP);

  pinMode(Autonoom_Knop, INPUT_PULLUP);
  pinMode(Volg_Modus_Knop, INPUT_PULLUP);

  
  //Digital Writes
  digitalWrite(Standby_Pin_Rechts, HIGH);
  digitalWrite(Standby_Pin_Links, HIGH);
  digitalWrite(Ultrasoon_Voor_Trigger, LOW);
  digitalWrite(Ultrasoon_Links_Voor_Trigger, LOW);
  digitalWrite(Ultrasoon_Rechts_Voor_Trigger, LOW);
  digitalWrite(Ultrasoon_Links_Achter_Trigger, LOW);
  digitalWrite(Ultrasoon_Rechts_Achter_Trigger, LOW);
}

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+loop-+-+-+-+-+-+-+--+-+-+-+-+-+-+-++-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

void loop()
{

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++Switch toestanden+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

  switch(Stap)
  {
    case (Volg_Modus):
      Volg_Modus_Functie(); 
      break;
      
    case (Idle):
      Serial.println("IDLE");
      digitalWrite(Standby_Pin_Links, LOW);
      digitalWrite(Standby_Pin_Rechts, LOW);
      break;
      
    case (Rijden):
      Serial.println("Rijden");
      digitalWrite(Standby_Pin_Rechts, HIGH);
      digitalWrite(Standby_Pin_Links, HIGH);
      for (int r=0;r<30;r++)
      {
      //digitalWrite(Standby_Pin_Rechts, HIGH);
      //digitalWrite(Standby_Pin_Links, HIGH);
      Stepper_Rechts.step(1);
      Stepper_Links.step(1);
      }
      break;
      
    case (Actie_Proces_Gewas):
      Serial.println("Actie_Proces_Gewas");
      digitalWrite(Standby_Pin_Links, LOW);
      digitalWrite(Standby_Pin_Rechts, LOW);
      delay(500);
      digitalWrite(Standby_Pin_Rechts, HIGH);
      digitalWrite(Standby_Pin_Links, HIGH);
      
      digitalWrite(LedPins, HIGH);
      for(int r=0; r<200;r++)
      {
        Stepper_Rechts.step(1);
        Stepper_Links.step(1);
      }
      digitalWrite(LedPins, LOW);
      digitalWrite(Standby_Pin_Links, LOW);
      digitalWrite(Standby_Pin_Rechts, LOW);
      delay(500);
      break;
      
    case (Actie_Proces_Object):
      Serial.println("Actie_Proces_Object");
      digitalWrite(Standby_Pin_Links, LOW);
      digitalWrite(Standby_Pin_Rechts, LOW);
      delay(5);
      break;
      
    case (Actie_Proces_Koers):
      Serial.println("Actie_Proces_Koers "); Serial.print("Bocht: "); Serial.println(Bocht);
      if(digitalRead(Autonoom_Knop) == LOW || digitalRead(Volg_Modus_Knop) == HIGH)
      {
        Stap = Idle;
      }
      if(Bochten_Patroon[Bocht] == Linksom)
      {
        Bocht_linksom();
        Bocht_linksom();
      }
      if(Bochten_Patroon[Bocht] == Rechtsom)
      {
        Bocht_rechtsom();
        Bocht_rechtsom();
      }
      break;
      
     case (Actie_Proces_Koers_Correctie): //----------------------------------------------------------------
      Serial.println("Actie_Proces_Koers_Correctie");
      if(Correctie_Links == 1)
      {
        Stepper_Rechts.step(3);
        Correctie_Links = 0;
      }
      if(Correctie_Rechts == 1)
      {
        Stepper_Links.step(3);
        Correctie_Rechts = 0;
      }
      break;
  }

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++Switch overgangen+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
  
  switch(Stap)
  {
    case (Volg_Modus):
      if(digitalRead(Volg_Modus_Knop) == LOW)
      {
        Stap = Idle;
      }
      break;
      
    case (Idle): //------------------------------------------------------------------
      if(digitalRead(Autonoom_Knop))
      {
        Stap = Rijden;
        Bocht = -1; 
      }
      if(digitalRead(Volg_Modus_Knop))
      {
        Stap = Volg_Modus;
      }
      break;

/*------------------------------------------------------------case Rijden overgang-------------------------------------------------------------*/      
      
    case (Rijden): //--------------------------------------------------------------------
      if(digitalRead(Autonoom_Knop) == LOW)
      {
        Stap = Idle;
      }
      
      if(digitalRead(Eindstand_Pin_Links_Klein) == LOW)
      {
        Stap = Actie_Proces_Koers_Correctie;
        Correctie_Rechts = 1 ;
      }
      
      if(digitalRead(Eindstand_Pin_Rechts_Klein) == LOW)
      {
        Stap = Actie_Proces_Koers_Correctie;
        Correctie_Links = 1 ;
      }
      digitalWrite(Standby_Pin_Links, LOW);
      digitalWrite(Standby_Pin_Rechts, LOW);
      Distance = Distance_Cal(Ultrasoon_Voor_Trigger, Ultrasoon_Voor_Echo);
      if(Distance < Arm_Lengte && Distance > 5)
      {
        Stap = Actie_Proces_Object;
      }
      
      Stepper_Rechts.step(1);
      Stepper_Links.step(1);
      
      Distance = Distance_Cal(Ultrasoon_Links_Achter_Trigger, Ultrasoon_Links_Achter_Echo);
      if(Distance < Gewas_Afstand && Distance > 3)
      {
        Stap = Actie_Proces_Gewas;
      }
//      Stepper_Rechts.step(1);
//      Stepper_Links.step(1);
      
      Distance = Distance_Cal(Ultrasoon_Rechts_Achter_Trigger, Ultrasoon_Rechts_Achter_Echo);
      if(Distance < Gewas_Afstand && Distance > 3)
      {
        Stap = Actie_Proces_Gewas;
      }
      if((digitalRead(Eindstand_Pin_Rechts_Groot) == HIGH) && (digitalRead(Eindstand_Pin_Links_Groot) == HIGH))
      {
        Stap = Actie_Proces_Koers;
      }
      break;


/*------------------------------------------------------------case Actie_Proces_Gewas overgang-------------------------------------------------------------*/

    case (Actie_Proces_Gewas): //-------------------------------------------------------------
      Distance = Distance_Cal(Ultrasoon_Rechts_Achter_Trigger, Ultrasoon_Rechts_Achter_Echo);
      Serial.print("Distance proces gewas: "); Serial.println(Distance);      
      if(Distance > Gewas_Afstand || Distance == 0)
      {
        Stap = Rijden;
      }
      break;


/*------------------------------------------------------------case Actie_Proces_Obstakel overgang-------------------------------------------------------------*/
      
    case (Actie_Proces_Object): //--------------------------------------------------------------
      if(digitalRead(Autonoom_Knop) == LOW)
      {
        Stap = Idle;
      }
      Distance = Distance_Cal(Ultrasoon_Voor_Trigger, Ultrasoon_Voor_Echo);
      if(Distance > Arm_Lengte)
      {
        Stap = Rijden;
      }
      break;

/*------------------------------------------------------------case Actie_Proces_Koers overgang-------------------------------------------------------------*/     
      
    case (Actie_Proces_Koers): //----------------------------------------------------------------
      Bocht++;
      if(Bocht == 6)
      {
        Bocht = 0;
      }
      Stap = Rijden;
      break;
      
     case (Actie_Proces_Koers_Correctie): //----------------------------------------------------------------
      Stap = Rijden;
      break;
  }
}

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+/loop/-+-+-+-+-+-+-+--+-+-+-+-+-+-+-++-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+Distance_Cal-+-+-+-+-+-+-+--+-+-+-+-+-+-+-++-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

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

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+Bocht_Rechtsom-+-+-+-+-+-+-+--+-+-+-+-+-+-+-++-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

void Bocht_rechtsom()
{
  Serial.println("Bocht Rechtsom");
  for(int i = 0; i < 50; i++)
  {
    Distance = Distance_Cal(Ultrasoon_Voor_Trigger, Ultrasoon_Voor_Echo);
    //Serial.print("Bocht Rechtsom Distance:"); Serial.println(Distance);
    if(Distance < Arm_Lengte && Distance > 3)
    {
      digitalWrite(Standby_Pin_Rechts, LOW);
      digitalWrite(Standby_Pin_Links, LOW);
      i--;
    }
    else
    {
      digitalWrite(Standby_Pin_Rechts, HIGH);
      digitalWrite(Standby_Pin_Links, HIGH);
      Stepper_Rechts.step(1);
      Stepper_Links.step(1);
    }
  }
  digitalWrite(Standby_Pin_Rechts, HIGH);
  digitalWrite(Standby_Pin_Links, HIGH);
  for(int i = 0; i < 105; i++)
  {
     Stepper_Rechts.step(-1);
     Stepper_Links.step(1);
  }
  for(int i = 0; i < 120; i++)
  {
    Distance = Distance_Cal(Ultrasoon_Voor_Trigger, Ultrasoon_Voor_Echo);
    //Serial.print("Bocht Rechtsom Distance:"); Serial.println(Distance);
    if(Distance < Arm_Lengte && Distance > 3)
    {
      digitalWrite(Standby_Pin_Rechts, LOW);
      digitalWrite(Standby_Pin_Links, LOW);
      i--;
      
    }
    else
    {
      digitalWrite(Standby_Pin_Rechts, HIGH);
      digitalWrite(Standby_Pin_Links, HIGH);
      Stepper_Rechts.step(1);
      Stepper_Links.step(1);
    }
  }
}

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+Bocht_linksom-+-+-+-+-+-+-+--+-+-+-+-+-+-+-++-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

void Bocht_linksom()
{
  Serial.println("Bocht Linksom");
  for(int i = 0; i < 50; i++)
  {
    Distance = Distance_Cal(Ultrasoon_Voor_Trigger, Ultrasoon_Voor_Echo);
    if(Distance < Arm_Lengte && Distance > 3)
    {
      digitalWrite(Standby_Pin_Rechts, LOW);
      digitalWrite(Standby_Pin_Links, LOW);
      i--;
      
    }
    else
    {
      digitalWrite(Standby_Pin_Rechts, HIGH);
      digitalWrite(Standby_Pin_Links, HIGH);
      Stepper_Rechts.step(1);
      Stepper_Links.step(1);
    }
  }
  digitalWrite(Standby_Pin_Rechts, HIGH);
  digitalWrite(Standby_Pin_Links, HIGH);
  
  for(int i = 0; i < 105; i++)
  {
     Stepper_Rechts.step(1);
     Stepper_Links.step(-1);
  }
  for(int i = 0; i < 120; i++)
  {
    Distance = Distance_Cal(Ultrasoon_Voor_Trigger, Ultrasoon_Voor_Echo);
    //Serial.print("Bocht Linksom Distance:"); Serial.println(Distance);
    if(Distance < Arm_Lengte && Distance > 3)
    {
      digitalWrite(Standby_Pin_Rechts, LOW);
      digitalWrite(Standby_Pin_Links, LOW);
      i--;
      
    }
    else
    {
      digitalWrite(Standby_Pin_Rechts, HIGH);
      digitalWrite(Standby_Pin_Links, HIGH);
      Stepper_Rechts.step(1);
      Stepper_Links.step(1);
    }
  }
}

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+Volg_Modus_Functie-+-+-+-+-+-+-+--+-+-+-+-+-+-+-++-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

void Volg_Modus_Functie()
{
  
  Distance_Voor_Rechts = Distance_Cal(Ultrasoon_Rechts_Voor_Trigger, Ultrasoon_Rechts_Voor_Echo);
  Distance_Voor_Midden = Distance_Cal(Ultrasoon_Voor_Trigger, Ultrasoon_Voor_Echo);
  Distance_Voor_Links = Distance_Cal(Ultrasoon_Links_Voor_Trigger, Ultrasoon_Links_Voor_Echo);
  
  if(Distance_Voor_Rechts == 0)
  {
    Distance_Voor_Rechts = 500;
  }
  
  if(Distance_Voor_Midden == 0)
  {
    Distance_Voor_Midden = 500;
  }

  if(Distance_Voor_Links == 0)
  {
    Distance_Voor_Links = 500;
  }

  if((Distance_Voor_Rechts < Arm_Lengte && Distance_Voor_Rechts > 1) ||  (Distance_Voor_Midden < Arm_Lengte && Distance_Voor_Midden > 1) || (Distance_Voor_Links < Arm_Lengte && Distance_Voor_Links > 1))
  {
    digitalWrite(Standby_Pin_Links, LOW);
    digitalWrite(Standby_Pin_Rechts, LOW);
  }
  else
  {
    digitalWrite(Standby_Pin_Links, HIGH);
    digitalWrite(Standby_Pin_Rechts, HIGH);

    if(Distance_Voor_Midden > Distance_Voor_Links - 10 && Distance_Voor_Midden > Distance_Voor_Rechts - 10)
    {
      for(int i = 0; i < 50; i++)
      {
        Stepper_Rechts.step(1);
        Stepper_Links.step(1);
      }
    }
    else
    {
      if(Distance_Voor_Links < Distance_Voor_Rechts + 10)//wat als beide geen value hebben? dan komt er wss een willk adres en kan dit willk gekozen worden door de agv
      {
        for(int i = 0; i < 50; i++)
        {
          Stepper_Rechts.step(2);
          Stepper_Links.step(1);
        } 
      }
      if(Distance_Voor_Rechts < Distance_Voor_Links + 10)
      {
        for(int i = 0; i < 50; i++)
        {
          Stepper_Rechts.step(1);
          Stepper_Links.step(2);
        }
      }
    }
  }
}
