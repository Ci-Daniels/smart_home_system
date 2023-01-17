# smart_home_system
A smart system used to turn the garden lights on/ off, control the opening and closing of an underground tank and smart air conditioning system using Arduino and picsimlab for simulations.

## COMPONENTS
- Heater
- Fan
- LDR
- Serial Tank
- atmega328p microcontroller
- picsimlab
- Blynk application

### Picsimlab
- Is a realtime emulator for development boards with the avr-gdb debugger integration. - - Its purpose is to emulate real hardware and does not give a provision for any source code editting support. The code editting and debugging can be done on the specific ide being used. 
- The compiled binary code generated after compilation in the ide is exported to the picsimlab and then run to implement the process.

#### Heater and cooler system
- The picsimlab provides a platform to control heater and cooler systems. The systems works such that when the room temperature rises above 35 degrees celcius, the cooler automatically starts to reduce this said temperature.

#### Serial tank
- The serial tank has an inlet and outlet such that the inlet automatically opens when the water level is below 2000L and the outlet closes. Both the inlet and outlets can be opened and closed via a button on the blynk application.

#### LDR 
The ldr works such that when the resistance is high, the garden lights(led) will signifying that there is no sunlight outside. When the sun is up, the resistance on the ldr decreases and in turn the garden lights are switched off(led turns off).

#### CLCD
- The clcd is used to display the status of the heater that is the temperature as well as the volume of water in the tank which is also reflected on the bluynk application terminal.

#### Blynk application
- The system uses blynk application to control the heater, cooler and serial tank and records the status of the temperature and water volume on the  blynk terminal.

## CONCLUSION
- The simulation on the picsimlab was conclusive and the data was recorded in the power point document provided. 
