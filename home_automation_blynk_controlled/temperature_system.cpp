#include "temperature_system.h"
#include "Arduino.h"
#include "main.h"


void init_temperature_system(void)
{
    /*SETTING THE COOLER AND HEATER PINS AS OUTPUT PINS*/
    pinMode(HEATER, OUTPUT);
    pinMode(COOLER, OUTPUT);

    /*SETTING THE PINS TO OFF BY DEFAULT*/
    digitalWrite(HEATER, LOW);
    digitalWrite(COOLER, LOW);
}

float read_temperature(void)
{
  /*READ TEMPERATURE FROM THETEMPERATURE SENSOR*/
  float temperature;   

  //reading the analog output voltage fromtemperature sensor and converting it to deg cel (10mv-> 1deg cel)
  temperature =(((analogRead(A0) *(float)5/ 1024)) /(float) 0.01);

  return temperature;
}

void cooler_control(bool control)
{
   digitalWrite(COOLER , control);//assign the control from the blynk app
}
void heater_control(bool control)
{
    digitalWrite(HEATER , control);
}