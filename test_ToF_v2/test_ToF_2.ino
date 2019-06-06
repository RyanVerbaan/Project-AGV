

/*Dit script is ontworpen om een functie te vinden de afstand tot de van de Time of Flight 
(afgekort: ToF) sensor met als variabele de verkregen "range" van de Time of Flight sensor
Hiervoor wordt in de loop per meting een teller bijgehouden. Per iteratie wordt de gevonden 
range opgeteld bij de totale_waarde. Door dit te delen door te teller
wordt een gemiddelde hoeveelheid range gevonden. 

Het aansluitschema horend bij deze proef valt te vinden op deze link: 
https://learn.adafruit.com/adafruit-vl6180x-time-of-flight-micro-lidar-distance-sensor-breakout/wiring-and-test

De gebruikte ToF sensor is: adafruit vl6180x

De proef gaat als volgt: Zet het te meten object op 0 cm tot 20 cm (het minimale/maximale 
bereik) afstand van de ToF sensor. Druk op de reset knop en wacht tot er op zijn minst 100 
metingen zijn gemaakt; teller >= 100. Noteer het gevondde gemiddelde en ga door naar de
volgende afstand. Druk na het aanpassen van de afstand altijd op de reset-knop van de arduino. 
Meet alle gewensde afstanden in dit bereik. Uit de gevonden waardes kan  een functie tussen 
range en afstand worden geschat.*/

#include <Wire.h>
#include <VL53L0X.h>
//#include "Adafruit_VL6180X.h"


  float totale_waarde = 0;
  float teller = 0;
  float gemiddelde = 0;

#define SensorLreset 7
#define SensorRreset 6
  
VL53L0X sensorL;
//VL53L0X sensorR;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  pinMode(SensorLreset, OUTPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
//  pinMode(SensorRreset, OUTPUT);
  
  digitalWrite(SensorLreset, LOW);
//  digitalWrite(SensorRreset, LOW);
  delay(10);
  digitalWrite(SensorLreset, HIGH);
//  digitalWrite(SensorRreset, HIGH);
  delay(10);
  
//  digitalWrite(SensorRreset, LOW);
  sensorL.setAddress(0x30);
  sensorL.init();
  sensorL.setTimeout(500);

//  digitalWrite(SensorLreset, LOW);
//  sensorR.init();
//  sensorR.setAddress(0x31);
//  sensorR.setTimeout(500);
}
  

void loop() {
  Serial.println("1:   ");
  float SensorWaarde = sensorL.readRangeSingleMillimeters();
  Serial.print(SensorWaarde);
  if (sensorL.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
//
//  Serial.println("2:   ");
//  Serial.print(sensorR.readRangeSingleMillimeters());
//  if (sensorR.timeoutOccurred()) { Serial.print(" TIMEOUT"); }

}
