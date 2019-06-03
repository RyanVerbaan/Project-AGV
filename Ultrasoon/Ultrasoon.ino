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

#define Voor_Led 21
#define Links_Voor_Led 20
#define Rechts_Voor_Led 18
#define Gewas_Led 19

#define Arm_Lengte 80
#define Gewas_Afstand 100

float Distance = 0;
long Distance_Gem = 0;
float Distance_2 = 0;
long Distance_2_Gem = 0;
int Stap = 1;

#define Rijden 0
#define Volgmodus 1



void setup() {
  Serial.begin(9600);
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

  digitalWrite(Voor_Led, LOW);
  digitalWrite(Links_Voor_Led, LOW);
  digitalWrite(Rechts_Voor_Led, LOW);
  digitalWrite(Gewas_Led, LOW);
}

int Distance_Cal(int trigPin, int echoPin)
{
  float duration;
  float distance;
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration* 34/2;
  return distance;
}


void loop() {
Serial.println("--------------------------");
switch(Stap)
{ 
  case (Rijden):
    Distance = Distance_Cal(Ultrasoon_Voor_Trigger, Ultrasoon_Voor_Echo);
    Serial.print("Distance: ");
    Serial.println(Distance);
    if(Distance < Arm_Lengte || Distance > 0)
    {
    digitalWrite(Voor_Led, HIGH);  
    } 
//    else
//    {
//      digitalWrite(Voor_Led, LOW);  
//    }

    Distance = Distance_Cal(Ultrasoon_Links_Achter_Trigger, Ultrasoon_Links_Achter_Echo);
    Serial.print("Distance: ");
    Serial.println(Distance );
    
    if(Distance < Gewas_Afstand || Distance > 0)
    {
      digitalWrite(Gewas_Led, HIGH);  
    }
//    else
//    {
//      digitalWrite(Gewas_Led, LOW);
//    }

    Distance = Distance_Cal(Ultrasoon_Rechts_Achter_Trigger, Ultrasoon_Rechts_Achter_Echo);
    Serial.print("Distance: ");
    Serial.println(Distance);
    if(Distance < Gewas_Afstand || Distance > 0)
    {
      digitalWrite(Gewas_Led, HIGH);  
    }
//    else
//    {
//      digitalWrite(Gewas_Led, LOW);
//    }
    break;

    
  case (Volgmodus):
    Distance = Distance_Cal(Ultrasoon_Voor_Trigger, Ultrasoon_Voor_Echo);
    Serial.print("Distance: ");
    Serial.println(Distance );
    if(Distance < Arm_Lengte || Distance > 0)
    {
    digitalWrite(Voor_Led, HIGH);  
    } 
//    else
//    {
//      digitalWrite(Voor_Led, LOW);  
//    }

    Distance_2 = Distance_Cal(Ultrasoon_Links_Voor_Trigger, Ultrasoon_Links_Voor_Echo);
    Serial.print("Distance: ");
    Serial.println(Distance_2 );
    if(Distance_2 < Distance || Distance > 0)
    {
      digitalWrite(Links_Voor_Led, HIGH);  
    }
//    else
//    {
//      digitalWrite(Links_Voor_Led, LOW);
//    }

    Distance_2 = Distance_Cal(Ultrasoon_Rechts_Voor_Trigger, Ultrasoon_Rechts_Voor_Echo);
    Serial.print("Distance: ");
    Serial.println(Distance_2 );
    if(Distance_2 < Distance || Distance > 0)
    {
      digitalWrite(Rechts_Voor_Led, HIGH);  
    }
//    else
//    {
//      digitalWrite(Rechts_Voor_Led, LOW);
//    }
    break;
}
}
