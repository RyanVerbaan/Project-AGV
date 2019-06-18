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

float Distance = 0;

void setup() {
  Serial.begin(9600);
  Serial.print("Setup---------------------------------");
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
}

int Distance_Cal(int trigPin, int echoPin)
{
  float duration;
  float distance;
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration* 0.34/2;
  return distance;
}


void loop() {
Serial.println("--------------------------");

Distance = Distance_Cal(Ultrasoon_Voor_Trigger, Ultrasoon_Voor_Echo);
Serial.print("Distance Voor:   ");
Serial.println(Distance);

Distance = Distance_Cal(Ultrasoon_Links_Voor_Trigger, Ultrasoon_Links_Voor_Echo);
Serial.print("Distance Links Voor:   ");
Serial.println(Distance);

Distance = Distance_Cal(Ultrasoon_Rechts_Voor_Trigger, Ultrasoon_Rechts_Voor_Echo);
Serial.print("Distance Rechts Voor:   ");
Serial.println(Distance);

Distance = Distance_Cal(Ultrasoon_Links_Achter_Trigger, Ultrasoon_Links_Achter_Echo);
Serial.print("Distance Links Achter:   ");
Serial.println(Distance);

Distance = Distance_Cal(Ultrasoon_Rechts_Achter_Trigger, Ultrasoon_Rechts_Achter_Echo);
Serial.print("Distance Rechts Achter:   ");
Serial.println(Distance);


}
