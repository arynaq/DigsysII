#include "Arduino.h"

class PID{
  /* The value which the PID will try to converge towards */
  float* setPoint;
  /* The current value */
  float* newPoint;
  /* The previous value */
  float* prevPoint;
  /* The difference between the two */
  float* error;
  /* The update rate of the PID
     This should be much higher (lower frequency) than the sampling rate of newpoint
  */
  float dt;
  /*Simple timer, keep track of time between calls to update */
  float timer = 0;
  /*Output of PID */
  float out;

  /*Sum of integrand in PID algo*/
  float sum = 0;



  /*Default values for the algo coefficients */
  float KI = 0.1;
  float KD = 0.05;
  float KP = 1;
public:
  PID(float* s, float* n, float _samplerate){
    setPoint = s;
    newpoint = n;
    samplerate = _samplerate;
  }

  void update(){
    timer += millis();
    *error = *setPoint - *newPoint;
    if(timer>=dt){

      out = potential() + integrand() + derivative();


      sum+= error*dt;
      out = KP * (error) + KI*sum + KD*error/dt;

      prevPoint = *newPoint;
      timer = 0;
    }
  }

  float potential(){
    return KP*(error);
  }

  float derivative(){
    /* This value will spike massively if the error is big since dt is small
      but derror/dt = d(setpoint -newpoint)/dt but since the setpoint is constant
      derror/dt = -dnewpint/dt
    */

    return -KD*(*newPoint-prevPoint)/dt;
  }

  float integrand(){
    sum+= error*dt;
    return KI*sum;
  }

  inline float getKI(){return KI;}
  inline float getKD(){return KD;}
  inline float getKP(){return KP;}
  inline float getSetPoint(){return setPoint;}
  inline float getNewPoint(){return newPoint;}
  inline float out(){return out;}
  inline void  setKI(float _KI){KI = _KI;}
  inline void  setKD(float _KD){KD = _KD;}
  inline void  setKP(float _KP){KP = _KP;}
};