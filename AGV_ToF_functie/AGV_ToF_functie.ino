#include <Wire.h>
#include "Adafruit_VL6180X.h"

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
  return (gemiddelde-5.5)/11;//pas hier de gevonden functie toe uit de proef
}//ToF_meting

void setup() {
  Serial.begin(115200);
  // wait for serial port to open on native usb devices
  while (!Serial) {
    delay(1);
  }
}//setup

void loop() {
  //De variabele cm_tot_rand_boomgaard is hier toegevoegd omdat anders de functie 
  //ToF_meting() twee keer wordt aangeroepen en daarmee dus een verschillende meting wordt bekeken
  float cm_tot_rand_boomgaard = ToF_meting();
  Serial.print("afstand [cm]: "); 
  if(cm_tot_rand_boomgaard == 0){
     Serial.println("GEEN METING"); 
  }
  else{
    Serial.println(cm_tot_rand_boomgaard);
  }
  delay(10);
}//loop
