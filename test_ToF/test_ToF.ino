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
metingen zijn gemaakt; teller >= 100. Noteer het gevonden gemiddelde en ga door naar de
volgende afstand. Druk na het aanpassen van de afstandaltijd op de reset-knop van de arduino. 
Meet alle afstanden in dit bereik in gehele centimeters. Door de 21 gevonden waardes kan 
een functie tussen range en afstand voldoende nauwkeurig worden geschat.*/

#include <Wire.h>
#include "Adafruit_VL6180X.h"

Adafruit_VL6180X vl = Adafruit_VL6180X();

  float totale_waarde = 0;
  float teller = 0;
  float gemiddelde = 0;

void setup() {
  Serial.begin(115200);

  // wait for serial port to open on native usb devices
  while (!Serial) {
    delay(1);
  }
  Serial.println("Adafruit VL6180x test!");
  if (! vl.begin()) {
    Serial.println("Failed to find sensor");
    while (1);
  }
  Serial.println("Sensor found!");
}

void loop() {
  float lux = vl.readLux(VL6180X_ALS_GAIN_5);

  //Serial.print("Lux: "); Serial.println(lux);
  
  uint8_t range = vl.readRange();
  uint8_t status = vl.readRangeStatus();
  totale_waarde += range;
  teller+=1;
  gemiddelde = totale_waarde/teller;

  if (status == VL6180X_ERROR_NONE) {
    Serial.print("teller: "); Serial.print(teller); Serial.print("totale waarde: "); Serial.print(totale_waarde); Serial.print("Gemiddelde: "); Serial.println(gemiddelde);
  }
  

  // Some error occurred, print it out!
  
  if  ((status >= VL6180X_ERROR_SYSERR_1) && (status <= VL6180X_ERROR_SYSERR_5)) {
    Serial.println("System error");
  }
  else if (status == VL6180X_ERROR_ECEFAIL) {
    Serial.println("ECE failure");
  }
  else if (status == VL6180X_ERROR_NOCONVERGE) {
    Serial.println("No convergence");
  }
  else if (status == VL6180X_ERROR_RANGEIGNORE) {
    Serial.println("Ignoring range");
  }
  else if (status == VL6180X_ERROR_SNR) {
    Serial.println("Signal/Noise error");
  }
  else if (status == VL6180X_ERROR_RAWUFLOW) {
    Serial.println("Raw reading underflow");
  }
  else if (status == VL6180X_ERROR_RAWOFLOW) {
    Serial.println("Raw reading overflow");
  }
  else if (status == VL6180X_ERROR_RANGEUFLOW) {
    Serial.println("Range reading underflow");
  }
  else if (status == VL6180X_ERROR_RANGEOFLOW) {
    Serial.println("Range reading overflow");
  }

}
