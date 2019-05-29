
  //Aantal steps op de motor
#define STEPS     200
  //Pinnen voor linker stepper
#define Lpin_Ain2     4               //control pin
#define Lpin_Ain1     5               //control pin
#define Lpin_Bin1     6               //control pin
#define Lpin_Bin2     7               //control pin
#define Lpin_pwmA     4               //speed control pin
#define Lpin_pwmB     5               //speed control pin
  //Pinnen voor retcher stepper
#define Rpin_Ain1     28               //control pin
#define Rpin_Ain2     29               //control pin
#define Rpin_Bin1     30               //control pin
#define Rpin_Bin2     31               //control pin
#define Rpin_pwmA     6               //speed control pin
#define Rpin_pwmB     7               //speed control pin


int stap = 0;

void setup(){

  Serial.begin(9600);
  Serial.println("Stepper test!");
  pinMode(Lpin_Ain1, OUTPUT);
  pinMode(Lpin_Ain2, OUTPUT);
  pinMode(Lpin_Bin1, OUTPUT);
  pinMode(Lpin_Bin2, OUTPUT);
  pinMode(Lpin_pwmA, OUTPUT);
  pinMode(Lpin_pwmB, OUTPUT);
  pinMode(Rpin_Ain1, OUTPUT);
  pinMode(Rpin_Ain2, OUTPUT);
  pinMode(Rpin_Bin1, OUTPUT);
  pinMode(Rpin_Bin2, OUTPUT);
  //pinMode(Rpin_pwmA, OUTPUT);
  //pinMode(Rpin_pwmB, OUTPUT);

  //analogWrite(Lpin_pwmA, 255);
  //analogWrite(Lpin_pwmB, 255);
  //analogWrite(Rpin_pwmA, 255);
  //analogWrite(Rpin_pwmB, 255);


}

 
void loop()
{
  switch(stap){
    case(1):
      digitalWrite(Lpin_Ain1, HIGH);
      digitalWrite(Lpin_Bin2, LOW);
      digitalWrite(Lpin_Bin1, HIGH);
      digitalWrite(Lpin_Bin2, LOW);
      break;
    case(2):
      digitalWrite(Lpin_Ain1, LOW);
      digitalWrite(Lpin_Bin2, HIGH);
      digitalWrite(Lpin_Bin1, HIGH);
      digitalWrite(Lpin_Bin2, LOW);
      break;
    case(3):
      digitalWrite(Lpin_Ain1, LOW);
      digitalWrite(Lpin_Bin2, HIGH);
      digitalWrite(Lpin_Bin1, LOW);
      digitalWrite(Lpin_Bin2, HIGH);
      break;
    case(4):
      digitalWrite(Lpin_Ain1, HIGH);
      digitalWrite(Lpin_Bin2, LOW);
      digitalWrite(Lpin_Bin1, LOW);
      digitalWrite(Lpin_Bin2, HIGH);
      break;
  }  
  delay(1); 
  stap++;
  
}
