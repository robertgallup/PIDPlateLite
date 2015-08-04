//
// SMD Soldering Hotplate Controller
//
// Inspired by Tim Hirzel's Bare Bones Coffee Controller (2008)
//
// Modifications for hot plate SMD reflow by Scott Dixon, March 2010.
//
// Additional modifications to create PIDPlateLite by Robert Gallup (bob@robertgallup.com), May 2012
// * Removed thermocouple code
// * Removed EEPROM code
// * Removed Serial Monitor support
// * Updated UI significantly to simplify the PID Lite interaction experience
//
// All code released under
// Creative Commons Attribution-Noncommercial-Share Alike 3.0

// Hardware Connections:

// LCD:
// rs        : Pin 12
// rw        : Pin 11
// enable    : Pin 6
// backlight control on pin 5 (pwm 0=bright, 511=off)
// d4, d5, d6, d7 on pins 7, 8, 9, 10
// backlight pwm control is pin 5

// ROTARY ENCODER:
// Encoder 1 : Pin 2
// Encoder 2 : Pin 18
// Switch    : Pin 19

// IR TEMPERATURE SENSOR:
// IR Clock  : Pin 3
// IR Data   : Pin 4

// More information about the IR Sensor and SMD Hotplate is available at:
// http://dorkbotpdx.org/blog/scott_d/inexpensive_ir_based_temperature_sensor_for_microprocessors
// http://www.instructables.com/id/Closing-the-Loop-on-Surface-Mount-Soldering/

// Photos of my project (robert gallup) can be found at:
// http://www.instagram.com/robertgallup

// DEFAULT PID Values. Generally:
// P controls rise time (increase to reduce rise time, increase overshoot, and decrease solid state error)
// I controls steady-state error (increase to reduce error, but increase overshoot and settling time)
// D controls overshoot and settling time (increase to decrease overshoot, and decrease settling time)

#define DEFAULT_P 55
#define DEFAULT_I .12
#define DEFAULT_D 100

// PID Update Interval in milliseconds
const unsigned long PID_UPDATE_INTERVAL = 200;

// Default target temp
#define DEFAULT_TARGET_TEMP 100

float targetTemp;  //current temperature goal
float heatPower; // 0 - 1000  milliseconds on per second

unsigned long lastPIDTime;  // most recent PID update time in ms

void setup()
{

  targetTemp = DEFAULT_TARGET_TEMP;
  setupPID();

  setupLCD(targetTemp, true);
  lastPIDTime = millis();
  
  setupHeater();  
  setupTempSensor();

}

// Wrapper for targetTemp variable
float getTargetTemp() {
  return targetTemp;
}

// LOOP
void loop()
{  

  float new_temp;
  updateTempSensor();

  // Every update interval
  if ((millis() - lastPIDTime) > PID_UPDATE_INTERVAL) {
    
    new_temp = updateLCD(targetTemp, getLastTemp(), 0);
    if (new_temp > 0) targetTemp = new_temp;

    lastPIDTime +=  PID_UPDATE_INTERVAL;
    heatPower = updatePID(targetTemp, getFreshTemp());
    setHeatPowerPercentage(heatPower);

  }  

  updateHeater();

}
