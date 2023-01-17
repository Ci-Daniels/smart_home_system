/*************************************************************
Name          :   Kariithi Anne
Cohort        :   IOT B2
Title         :   Home automation using blynk
Description   :   To control light's brigntness with brightness,monitor temperature , monitor water level in the tank through blynk app
Pheripherals  :   Arduino UNO , Temperature system, LED, LDR module, Serial Tank, Blynk cloud, Blynk App.
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPLRYYIa-Vl"
#define BLYNK_DEVICE_NAME "Home Automation"
#define BLYNK_AUTH_TOKEN "qHystHqzze6HPVW8Bz-cCd6kEU4f3yQf"


// Comment this out to disable prints 
//#define BLYNK_PRINT Serial

#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "main.h"
#include "temperature_system.h"
#include "ldr.h"
#include "serial_tank.h"

char auth[] = BLYNK_AUTH_TOKEN;
bool heater_sw,inlet_sw,outlet_sw;
unsigned int tank_volume;

BlynkTimer timer;

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

/* This function is called every time the Virtual Pin 0 state changes
To turn ON and OFF cooler based virtual PIN value*/
BLYNK_WRITE(COOLER_V_PIN)
{
  int value = param.asInt();
  if (value)
  {
    /*TURNS THE COOLER ON IF THE ON BUTTON IN BLYNK IS ON*/
    cooler_control(ON);
    /*DISPLAYING THE NOTIFICATION OF THE STATE OF THE COOLER ON THE LCD*/
    lcd.setCursor(7,0);
    lcd.print(" CO_LR ON ");
  }
  else
  {
     /*TURNS THE COOLER OFF IF THE ON BUTTON IN BLYNK IS OFF*/
    cooler_control(OFF);
    lcd.setCursor(7,0);
    lcd.print(" C_LR OFF ");
  }
  
}
/*To turn ON and OFF heater based virtual PIN value*/
BLYNK_WRITE(HEATER_V_PIN )
{
  heater_sw = param.asInt();
  if ( heater_sw)
  {
    /*TURNS THE HEATER ON IF THE ON BUTTON IN BLYNK IS ON*/
    heater_control(ON);
    /*DISPLAYING THE NOTIFICATION OF THE STATUS OF THE HEATER ON THE LCD DISPLAY*/
    lcd.setCursor(7,0);
    lcd.print(" HT_R ON ");
  }
  else
  {
     /*TURNS THE HEATER OFF IF THE ON BUTTON IN BLYNK IS OFF*/
    heater_control(OFF);
    lcd.setCursor(7,0);
    lcd.print(" HT_R OFF ");
  }
}
/*To turn ON and OFF inlet vale based virtual PIN value*/
BLYNK_WRITE(INLET_V_PIN)
{
  inlet_sw = param.asInt();
  /*if inlet valve button is at logic high*/
  if(inlet_sw)
  {
    /*turn on the inlet valve*/
    enable_inlet();
    /*to print the status of valve on the clcd*/
    lcd.setCursor(7,1);
    lcd.print(" IN_FL_ON ");
   }
  else{
    /*else turn ofF*/
    disable_inlet();
    /*to print the status of valve on the clcd*/
    lcd.setCursor(7,1);
    lcd.print(" IN_FL_OFF ");
  }
}
/*To turn ON and OFF outlet value based virtual switch value*/
BLYNK_WRITE(OUTLET_V_PIN)
{
   outlet_sw = param.asInt();
   if(outlet_sw)
  {
    /*turn on the inlet valve*/
    enable_outlet();
    /*to print the status of valve on the clcd*/
    lcd.setCursor(7,1);
    lcd.print(" OT_FL_ON ");
   }
  else{
    /*else turn ofF*/
    disable_outlet();
    /*to print the status of valve on the clcd*/
    lcd.setCursor(7,1);
    lcd.print(" OT_FL_OFF ");
  }
}
/* To display temperature and water volume as gauge on the Blynk App*/  
void update_temperature_reading()
{
  // You can send any value at any time.
  Blynk.virtualWrite(TEMPERATURE_GAUGE, read_temperature());
  Blynk.virtualWrite(WATER_VOL_GAUGE, volume());
}

/*To turn off the heater if the temperature raises above 35 deg C*/
void handle_temp(void)
{
   /*compare temperature with 35 and check if heater is ON*/
   if((read_temperature() > float(35)) && heater_sw)
   {
    /*turn off heater switch*/
    heater_sw = 0;
    heater_control(OFF);

    /*print on the CLCD*/
    lcd.setCursor(7,0);
    lcd.print(" HT_R OFF ");

    /*to display notification on the blynk*/
     Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN, "Temperature is above 35 degree Celcius\n");
     Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN, "Turning off the heater!\n");

     /*To reflect OFF on the heater button*/
     Blynk.virtualWrite(HEATER_V_PIN,0);
   }
}
 /*To control water volume above 2000ltrs*/
void handle_tank(void)
{
  /*compare the volume of water within 2000l and check status of the inlet valve*/
  if( (tank_volume < 2000)&& (inlet_sw == OFF))
  {
    /*enable the inlet and print the status of the pin*/
    enable_inlet();
    /*to print the status of valve on the clcd*/
    lcd.setCursor(7,1);
    lcd.print(" IN_FL_ON ");
    inlet_sw = ON;

    /*update button status on the blynk application*/
    Blynk.virtualWrite( INLET_V_PIN , OFF);

    /*print notification on the terminal of blynk*/
     Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN, "Water volume is less thab 2000l\n");
     Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN, "Turning ON the inlet_valve!\n");
   }
   /*check if the tank is full then turn off the inlet valve*/
    if( (tank_volume == 3000) && (inlet_sw == ON))
  {
    /*enable the inlet and print the status of the pin*/
    disable_inlet();
    /*to print the status of valve on the clcd*/
    lcd.setCursor(7,1);
    lcd.print(" IN_FL_OFF ");
    inlet_sw = OFF;

    /*update button status on the blynk application*/
    Blynk.virtualWrite( INLET_V_PIN , OFF);

    /*print notification on the terminal of blynk*/
     Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN, "The tank is full\n");
     Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN, "Turning OFF the inlet_valve!\n");
   }
  
}

void setup(void)
{
    /*initialise the blynk;connecting arduino to the blynk server*/
    Blynk.begin(auth);
   /*initialize the lcd*/
    lcd.init();  
    /*turn the backlight */                   
    lcd.backlight();
    /*clear the clcd*/
    lcd.clear();
    /*cursor to the home */
    lcd.home();  
 
 /*set cursor to first line to display temperature*/
    lcd.setCursor(0,0);
    lcd.print("T = ");
    
  /*set cursor to second line to display volume of water*/
    lcd.setCursor(0,1);
    lcd.print("V = ");
    
  /*initialising garden lightsas output pin*/
    init_ldr();
  /*initialise the temperature system*/
    init_temperature_system();
 /*INITIALISING THE SERIAL TANK*/
    init_serial_tank();
    /*SET THE TIME INTERVAL FOR WHICH NOTIFICATIONS SHOULD BE SENT TO BLYNK*/
    /*update temperature reading on the blynk application*/
    timer.setInterval(1000, update_temperature_reading);
}

void loop(void) 
{
    /*controlling the brightness of the garden lights using LDR sensor*/
     brightness_control(); 
    /*read temp and display it*/
     lcd.setCursor(2,0);
     
     /*DISPLAYING THE VALUE OF THE TEMPERATUE GOT FROM FROM THE TEMPERATURE SENSOR*/
     String temperature;
     temperature = String(read_temperature(),2);
     /*type casting float temperature value to string*/
     lcd.print(temperature);

     /*display the volume of water on the CLCD*/
     tank_volume = volume();
     lcd.setCursor(2,1);
     lcd.print(tank_volume);

     /*to check the threshold temperature and controlling heater*/
     handle_temp();

     /*To monitor the volume of the waterand if less than 2000l, turn on the inlet valve*/
     handle_tank();
     
     /*To run  blynk application*/
     Blynk.run();
     timer.run();
    
}
