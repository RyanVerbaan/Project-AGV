#define Volg_Modus -1
#define Idle 0
#define Rijden 1
#define Actie_Proces_Gewas 2
#define Actie_Proces_Obstakel 3
#define Actie_Proces_Koers 4

#define Volgmodus -1
#define Autonoom 1

int Koers = 0;
#define Rechts_Afslaan 1
#define Links_Afslaan -1

#define Rij_Snelheid 512
#define Arm_Lengte 87,5 //milimeter 1/8 Schaalmodel van 0,7 Meter
#define Gewas_Afstand 50 //afstand van de gewassen tot de agv (misschien iets meer)
#define Koers_Value 10 //afstand van de rand van de gewassen tot agv
#define Koers_Marge 10 //hoeveelheid speelruimte van de koers van de agv

int Stap = 0;
int Status = 0;

#define Stepper_Links_Pin 1
#define Stepper_Rechts_Pin 2

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

void setup() 
{
  pinMode(Stepper_Links_Pin, OUTPUT);
  pinMode(Stepper_Rechts_Pin, OUTPUT);

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

  digitalWrite(Stepper_Links_Pin, LOW);
  digitalWrite(Stepper_Rechts_Pin, LOW);
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

void Actie_Proces_Gewas_Functie()
{
  digitalWrite(Stepper_Links_Pin, LOW);
  digitalWrite(Stepper_Rechts_Pin, LOW);
  delay(100);
  digitalWrite(Signaal_Ledjes, HIGH);
  delay(100);
  digitalWrite(Signaal_Ledjes, LOW);
}

void Actie_Proces_Obstakel_Functie()
{
  digitalWrite(Stepper_Links_Pin, LOW);
  digitalWrite(Stepper_Rechts_Pin, LOW);
}

void Actie_Proces_Koers_Functie()
{
  
}

void Volg_Modus_Functie()
{
  
}


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
