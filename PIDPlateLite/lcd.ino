#include <LiquidCrystal.h>
#include "RotaryEncoder.h"

// LiquidCrystal display with:
// rs on pin 12
// rw is pin 11
// enable on pin 6
// backlight control on pin 5 (pwm 0=bright, 511=off)
// d4, d5, d6, d7 on pins 7, 8, 9, 10
// backlight pwm control is pin 5
#define D4 7
#define D5 8
#define D6 9
#define D7 10
#define RW 11
#define E 6
#define RS 12
#define BL 5

#define LONG_PRESS_TIME 3000

typedef enum {R_NORMAL, R_LOOK_FOR_LONGPRESS, R_RESET} r_state;

LiquidCrystal lcd(RS, RW, E, D4, D5, D6, D7);
RotaryEncoder rotary(2, 19, 18);

// Press + long press stuff
boolean rotaryWaitingForRelease;
long rotaryPressStartTime;

int blevel = 80;
unsigned long startTime;

void setupLCD(float startTemp, boolean startLCD) {
  if (startLCD) {
    delay(1000);
    lcd.begin(2,16);
    lcd.clear();
    pinMode(BL, OUTPUT);
    analogWrite(BL,blevel);
    
    // Startup Screen
    lcd.print("PID Plate Lite  ");
    lcd.setCursor(0,1);
    lcd.print("V1.0            ");
    waitFor (3000);
    lcd.clear();

  }
  
  startTime = millis();
  rotary.minimum(20);
  rotary.maximum(280);
  rotary.position(startTemp);
  
  // Set up rotary states
  rotaryWaitingForRelease = false;

}

float updateLCD(float target, float t1, float t2) {
  float new_temp, trg;
  
  new_temp = -1;
  trg = target;

  if(rotary.pressed())
  {
    if (!rotaryWaitingForRelease) {
      rotaryWaitingForRelease = true;
      rotaryPressStartTime = millis();
    } else {
      long elapsedTime = millis() - rotaryPressStartTime;
      if (elapsedTime < 0) {
        rotaryPressStartTime = 0;
        elapsedTime = millis();
      };
      if (elapsedTime > LONG_PRESS_TIME) {
        setupLCD(DEFAULT_TARGET_TEMP, false);
        rotaryWaitingForRelease = false;
      }
    }
  
  // Set temperature on release
  } else {
    if (rotaryWaitingForRelease) {
      new_temp = rotary.position();
      trg = new_temp;
      startTime = millis(); // reset timer
      rotaryWaitingForRelease = false;
    }
  }
  

  //////////////////////////////////////////////////////////////////////
  // Elapsed time calculations
  int elapsedTime        = round ((millis() - startTime) / 1000);
  
  // Handle unlikely event of a rollover in millis counter (currently 49 days?)
  if (elapsedTime < 0) {
    startTime = 0;
    elapsedTime = millis();
  }
  
  // Calculate some elapsed time variables
  int elapsedTime_hours  = (int)floor(elapsedTime/3600) % 24;           // Rolls over after 23:59:59
  int elapsedTime_min    = floor((elapsedTime % 3600) / 60);
  int elapsedTime_sec    = elapsedTime % 60;
  
  // Print elapsed time on first line
  lcd.setCursor (8,0);
  if (elapsedTime_hours > 0) {
    if (elapsedTime_hours < 10) lcd.print (" ");
    lcd.print(elapsedTime_hours); lcd.print(":");
  } else {
    lcd.print("   ");
  }
  if (elapsedTime_min < 10) lcd.print ("0");
  lcd.print(elapsedTime_min  ); lcd.print(":");
  if (elapsedTime_sec < 10) lcd.print ("0");
  lcd.print(elapsedTime_sec  );
  
  // Update to Temperature
  lcd.setCursor(0, 0);
  int update_to_temp = rotary.position();
  if (update_to_temp == trg) {
    lcd.print("    ");  
  } else {
    printLeadingSpaces(update_to_temp, 3);
    lcd.print(update_to_temp);
    lcd.print(">");
  }    
    
  // Target Temperature
  lcd.setCursor(4, 0);
  int rTrg = round(trg);
  printLeadingSpaces (rTrg, 3);
  lcd.print(rTrg);
 
  // Measured Temperature
  lcd.setCursor(4, 1);
  int rT1 = round(t1);
  printLeadingSpaces(rT1, 3);
  lcd.print(rT1);

  return new_temp;

}

void printLeadingSpaces(int n, int w) {
  int ref=10;
  for (int i=1; i<w; i++) {
    if (n < ref) lcd.print(" ");
    ref *= 10;
  };
}

void updateLCDHeaterStatus (boolean on) {
  lcd.setCursor(15,1);
  lcd.print(on?"_":" ");
}

void waitFor (int m) {
  long startTime = millis();
  while ((millis() - startTime) < m) {
    if (rotary.pressed()) return;
  }
  return;
}

