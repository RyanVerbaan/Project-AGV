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

Actie_Proces_Gewas()
{
  digitalWrite(Stepper_Links_Pin, LOW);
  digitalWrite(Stepper_Rechts_Pin, LOW);
  delay(100);
  digitalWrite(Signaal_Ledjes, HIGH);
  delay(100);
  digitalWrite(Signaal_Ledjes, LOW);
}

Actie_Proces_Obstakel()
{
  digitalWrite(Stepper_Links_Pin, LOW);
  digitalWrite(Stepper_Rechts_Pin, LOW);
}

Bocht_Links()
{ //hoeveelheid_bochten++;
  digitalWrite(Stepper_Links_Pin, Lage_Bocht_Snelheid);
  digitalWrite(Stepper_Rechts_Pin, Hoge_Bocht_Snelheid);
}

Bocht_Rechts()
{ //ToF_switch, LOW)
  digitalWrite(Stepper_Links_Pin, Hoge_Bocht_Snelheid);
  digitalWrite(Stepper_Rechts_Pin, Lage_Bocht_Snelheid);  
}

Volg_Modus()
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
