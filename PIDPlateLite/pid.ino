
// PID control code
// Tim Hirzel
// December 2007

// This is a module that implements a PID control loop
// initialize it with 3 values: p,i,d
// and then tune the feedback loop with the setP etc funcs
//
// this was written based on a great PID by Tim Wescott:
// http://www.embedded.com/2000/0010/0010feat3.htm
//
//
// All code released under
// Creative Commons Attribution-Noncommercial-Share Alike 3.0 

#define WINDUP_GUARD_GAIN 100.0

float iState = 0;
float lastTemp = 0;
float pgain, igain, dgain;
float pTerm, iTerm, dTerm; 

#ifdef USE_EEPROM
int pgainAddress, igainAddress, dgainAddress;
#endif

void setupPID() {
  // with this setup, you pass the addresses for the PID algorithm to use to 
  // for storing the gain settings.  This way wastes 6 bytes to store the addresses,
  // but its nice because you can keep all the EEPROM address allocaton in once place.

  pgain = DEFAULT_P;
  igain = DEFAULT_I;
  dgain = DEFAULT_D;
}

float getP() {
  // get the P gain 
  return pgain;
}
float getI() {
  // get the I gain
  return igain;
}
float getD() {
  // get the D gain
  return dgain;
}

float updatePID(float targetTemp, float curTemp)
{
  // these local variables can be factored out if memory is an issue, 
  // but they make it more readable
  double result;
  float error;
  float windupGaurd;

  // determine how badly we are doing
  error = targetTemp - curTemp;

  // the pTerm is the view from now, the pgain judges 
  // how much we care about error we are this instant.
  pTerm = pgain * error;

  // iState keeps changing over time; it's 
  // overall "performance" over time, or accumulated error
  iState += error;

  // to prevent the iTerm getting huge despite lots of 
  //  error, we use a "windup guard" 
  // (this happens when the machine is first turned on and
  // it cant help be cold despite its best efforts)

  // not necessary, but this makes windup guard values 
  // relative to the current iGain
  windupGaurd = WINDUP_GUARD_GAIN / igain;  

  if (iState > windupGaurd) 
    iState = windupGaurd;
  else if (iState < -windupGaurd) 
    iState = -windupGaurd;
  iTerm = igain * iState;

  // the dTerm, the difference between the temperature now
  //  and our last reading, indicated the "speed," 
  // how quickly the temp is changing. (aka. Differential)
  dTerm = (dgain* (curTemp - lastTemp));

  // now that we've use lastTemp, put the current temp in
  // our pocket until for the next round
  lastTemp = curTemp;

  // the magic feedback bit
  return  pTerm + iTerm - dTerm;
}
