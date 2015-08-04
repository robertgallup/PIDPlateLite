# PIDPlateLite
- - -

### Code for a closed-loop-controlled hotplate SMD soldering station.

**Author:** Robert Gallup (robertgallup.com, github.com/robertgallup)  
**Date:** 8/4/2015  
**Version:** 1.0  

- - -

Inspired by Tim Hirzel's Bare Bones Coffee Controller (2008)

Modifications for hot plate SMD reflow by Scott Dixon, March 2010.

Additional modifications to create PIDPlateLite by Robert Gallup (bob@robertgallup.com), May 2012

* Removed thermocouple code
* Removed EEPROM code
* Removed Serial Monitor support
* Updated UI significantly to simplify the PID Lite interaction experience

All code released under Creative Commons Attribution-Noncommercial-Share Alike 3.0

Hardware Connections:

**LCD**  
rs        : Pin 12  
rw        : Pin 11  
enable    : Pin 6  
backlight control on pin 5 (pwm 0=bright, 511=off)  
d4, d5, d6, d7 on pins 7, 8, 9, 10  
backlight pwm control is pin 5  

**ROTARY ENCODER**  
Encoder 1 : Pin 2  
Encoder 2 : Pin 18  
Switch    : Pin 19

**IR TEMPERATURE SENSOR**  
IR Clock  : Pin 3  
IR Data   : Pin 4

More information about the IR Sensor and SMD Hotplate is available at:

_http://dorkbotpdx.org/blog/scott_d/inexpensive_ir_based_temperature_sensor_for_microprocessors_  
_http://www.instructables.com/id/Closing-the-Loop-on-Surface-Mount-Soldering/_

Photos of this project can be found at:

http://www.instagram.com/robertgallup
