#include "Arduino.h"

class PID{
  /* The value which the PID will try to converge towards */
  float* setPoint;
  /* The current value */
  float* newPoint;
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




  float KI;
  float KD;
  float KP;
public:
  PID(float* s, float* n, float _samplerate){
    setPoint = s;
    newpoint = n;
    samplerate = _samplerate;
  }

  void update(){
    timer += millis();
    *error = *newpoint - *setPoint;
    if(timer>=dt){
      sum+= error*dt;
      out = KP * (error) + KI*sum + KD*error/dt;
      timer = 0;
    }
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