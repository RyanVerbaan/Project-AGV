#define Volg_Modus -1
#define Idle 0
#define Rijden 1
#define Actie_Proces_Gewas 2
#define Actie_Proces_Obstakel 3
#define Actie_Proces_Koers 4

#define Autonoom -1
#define Volgmodus 1

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
#define Ultrasoon_Voor 3
#define Ultrasoon_Links_Voor 4
#define Ultrasoon_Rechts_Voor 5
#define Ultrasoon_Links_Achter 6
#define Ultrasoon_Rechts_Achter 7
#define Time_Of_Flight_Links 8
#define Time_Of_Flight_Rechts 9
#define Signaal_Ledjes 10

void setup() 
{
pinMode(Stepper_Links_Pin, OUTPUT);
pinMode(Stepper_Rechts_Pin, OUTPUT);

pinMode(Ultrasoon_Voor, INPUT);
pinMode(Ultrasoon_Links_Voor, INPUT);
pinMode(Ultrasoon_Rechts_Voor, INPUT);
pinMode(Ultrasoon_Links_Achter, INPUT);
pinMode(Ultrasoon_Rechts_Achter, INPUT);

pinMode(Time_Of_Flight_Links, INPUT);//AANPASSEN? ToF links en rechts op zelfde pin met switches die omschakelen na het geheel uitvoeren van een bochtfunctie
pinMode(Time_Of_Flight_Rechts, INPUT);

pinMode(Signaal_Ledjes, OUTPUT);

digitalWrite(Stepper_Links_Pin, LOW);
digitalWrite(Stepper_Rechts_Pin, LOW);
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
      if(Koers == Links_Afslaan)
      Actie_Proces_Koers_Functie();
      if(Koers == Rechts_Afslaan)
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

      if(digitalRead(Ultrasoon_Links_Achter) > Gewas_Afstand || digitalRead(Ultrasoon_Rechts_Achter) > Gewas_Afstand); //moet nog vergeleken met een waarde maar niet op HIGH
      Stap = Actie_Proces_Gewas;
      
      if(digitalRead(Ultrasoon_Links_Voor) == HIGH || digitalRead(Ultrasoon_Voor) == HIGH || digitalRead(Ultrasoon_Rechts_Voor) == HIGH); //moet nog vergeleken met een waarde maar niet op HIGH
      Stap = Actie_Proces_Obstakel;
      
      if(digitalRead(Time_Of_Flight_Links) > Koers_Value + Koers_Marge || digitalRead(Time_Of_Flight_Links) < Koers_Value - Koers_Marge)
      Stap = Actie_Proces_Koers;
      break;
      
    case (Actie_Proces_Gewas):
      //stepper uit
      digitalWrite(Signaal_Ledjes, HIGH);
      delay(1000);
      digitalWrite(Signaal_Ledjes, LOW);
      Stap = Rijden;
      break;
      
    case (Actie_Proces_Obstakel):
      if(digitalRead(Ultrasoon_Links_Voor) > Arm_Lengte && digitalRead(Ultrasoon_Voor) > Arm_Lengte && digitalRead(Ultrasoon_Rechts_Voor) > Arm_Lengte);
      Stap = Rijden;
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
