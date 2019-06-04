//https://www.arduino.cc/en/Reference/Wire
//https://learn.adafruit.com/adafruit-vl6180x-time-of-flight-micro-lidar-distance-sensor-breakout/wiring-and-test

#include <Wire.h>
#include "Adafruit_VL6180X.h"

#define links 10
#define rechts 11
uint8_t teller = 0;

Adafruit_VL6180X vl = Adafruit_VL6180X();

float ToF_meting(){
  float totale_waarde = 0;
  float gemiddelde = 0;
  for(int i =0; i < 10; i++){
    
    uint8_t range = vl.readRange();
    uint8_t status = vl.readRangeStatus();
    if (status != VL6180X_ERROR_NONE) {
      return 0;
    }
    totale_waarde += range;
  }//for
  
  gemiddelde = totale_waarde/10;
  return (1.09395*(gemiddelde-5.5)/11+1.399406);//pas hier de gevonden functie toe uit de proef
}//ToF_meting

void setup() {
  Serial.begin(115200);
  pinMode(links, OUTPUT);
  pinMode(rechts, OUTPUT);
  digitalWrite(rechts, HIGH);
  digitalWrite(links, LOW);
}//setup

void loop() {
  //De variabele cm_tot_rand_boomgaard is hier toegevoegd omdat anders de functie 
  //ToF_meting() twee keer wordt aangeroepen en daarmee dus een verschillende meting wordt bekeken
  teller++;
  if(teller % 200 < 100){
    digitalWrite(rechts, HIGH);
    digitalWrite(links, LOW);
    Serial.print("R ");
  } else{
    digitalWrite(rechts, LOW);
    digitalWrite(links, HIGH);
    Serial.print("L ");
  }
  float cm_tot_rand_boomgaard = ToF_meting();
  Serial.print(teller);
  Serial.print(" afstand [cm]: "); 
  if(cm_tot_rand_boomgaard == 0){
     Serial.println("GEEN METING"); 
  }
  else{
    Serial.println(cm_tot_rand_boomgaard);
  }
}//loop
