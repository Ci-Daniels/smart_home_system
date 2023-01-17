#include "ldr.h"
#include "Arduino.h"
#include "main.h"

void init_ldr(void)
{
   pinMode(GARDEN_LIGHT, OUTPUT);
   
}

unsigned int input_value = 0;

void brightness_control(void)
{
  /*read the ldr analog values and */
    input_value = analogRead( LDR_SENSOR );
     
  /*reverse map the from 1023 to 255*/
    input_value =  (1023 - input_value)/4;

  /*write the values*/
    analogWrite (GARDEN_LIGHT  , input_value);

    delay (100);
  
}
