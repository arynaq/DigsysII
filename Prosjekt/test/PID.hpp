#include "Arduino.h"
#include <math.h>

class PID{
  public:
  /*
  KP proportional constant of PID
  */
  float KP;
  /*
  KD derivative term of PID
  */
  float KD;
  /*
  KI integrator term of PID
  */
  float KI;

  /*
  Constructor, output is mirrored, newpoint is mirrored, setpoint is mirrored
  */

  PID(float* output, float* newPoint, float* setPoint,
         float dt, float KP, float KI, float KD){
    this->output = output;
    this->newPoint = newPoint;
    this->setPoint = setPoint;
    this->dt = dt;
    this->KP = KP;
    this->KI = KI;
    this->KD = KD;
  }


  /*Update the PID output, called in some loop where newPoint is updated */
  void update(){
    /*
    Update timer
    */
    timer+=millis();
    /*
    Update error, difference between setpoint and newpoint.
    */
    error = (*setPoint - *newPoint);

    /*
    If dt time has passed, apply PID
    */
    if(timer>= dt){
      /*
      The value of the out is updated
      */
      *output = proportional() + integrator() + derivative();
      /*
      timer is decreased by dt, timer MIGHT be 1.5 dt
      so we can't start counting at 0 again
      */
      timer -= dt;

      /*
      Constrain the PID output
      */
      if(*output>PID_MAX) *output = PID_MAX;
      if(*output<PID_MIN) *output = PID_MIN;
    }
    prevPoint = *newPoint;
    prevError = error;
  }

  void setConstraints(float _PID_MIN, float _PID_MAX, float _ERROR_MAX){  
    this->PID_MIN = _PID_MIN;
    this->PID_MAX = _PID_MAX;
    this->ERROR_MAX = _ERROR_MAX;
  }

  void setKP(float _KP){
    KP  = _KP;
  }
  void init(){
    timer = 0;
    prevPoint = *setPoint;
    prevError = 0;
  }

  float getError(){
    return error;
  }

private:
  /*
  The error, difference between new point and the setpoint
  */
  float error;
  /*
  The previous data point
  */
  float prevPoint;
  /*The difference in error */
  float prevError;
  /*
  The setpoint, the PID will try to converge towards this
  */
  float* setPoint;
  /*
  The new datapoint
  */
  float* newPoint;
  /*
  The PID output
  */
  float* output; 
  /*
  The sum in the integrator of PID
  */
  float sum = 0;
  /*
  The sampletime, in seconds, usually 0.1, time between outputs from PID
  */
  float dt;
  /*
  Keeps track of the time 
  */
  int timer;
  /*
  Maximum possible error, default PI since we measure angles
  */
  float ERROR_MAX = 3.141592/4;
  /*
  The minimum value of the PID, constrained to be no smaller than this
  */
  float PID_MIN = -100;

  /*
  The maximum value of the PID, constrained to be no bigger than this
  */
  float PID_MAX = 100;


  /*The integrator of the PID, notice how it is constrained, this is to prevent the PID
  from giving ouputs that are too big
  */
  float integrator(){
    sum += KI * error;
    if(sum>PID_MAX) sum = PID_MAX;
    if(sum<PID_MIN) sum = PID_MIN;

    return sum;
  }

  /*
  The derivative of the PID. Notice how we define it as the change between the new point and the previous point
  This is because if the newpoint is large compared to the setpoint the derivative will grow very large
  So:
    dError/dt = d(setPoint - newPoint) / dt = -(newPoint - prevPoint)/dt
    since setpoint is a constant it vanishes
  */
  float derivative(){
    //return KD/dt * (error - prevError);
    return - KD/dt * (*newPoint - prevPoint);
  }

  /*
  The proportional term of the PID
  */
  float proportional(){
    return KP * error; 
  }
 
};