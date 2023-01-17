#include "serial_tank.h"
#include "Arduino.h"
#include "main.h"

unsigned int volume_value;
unsigned char valueh, valuel;

void init_serial_tank(void) 
{ 
  //initialisation of the serial tank
    Serial.begin(19200);
    Serial.write(0xFF); 
    Serial.write(0xFF);
    Serial.write(0xFF);   
}

/*To read volume of water from the serial tank and retur it*/
unsigned int volume(void)
{
    /*sending command to request volume of water*/
    Serial.write(VOLUME);
    /*waiting for serial data*/
    while(!Serial.available());
    /*gets the higher byte value and store it in High*/
    valueh = Serial.read();
    while(!Serial.available());
    /*gets lower byte and store it in Low*/
    valuel = Serial.read();
    /*shift the first value by 8 digits and or it with the value stored in LOW*/
    volume_value = (valueh << 8) | valuel ; 
    
    return volume_value;
}


void enable_inlet(void)
{  
    /*enabling the inlet valve*/
    Serial.write(INLET_VALVE);
    Serial.write(ENABLE);
}  
void disable_inlet(void)
{   
    /*disables the inlet valve*/
    Serial.write(INLET_VALVE);
    Serial.write(DISABLE); 
}  
void enable_outlet(void)
{  
    /*enables the outlet valve*/
    Serial.write(OUTLET_VALVE);
    Serial.write(ENABLE);
}
void disable_outlet(void)
{  
    /*disables the outlet valve*/
    Serial.write(OUTLET_VALVE);
    Serial.write(DISABLE);
}
