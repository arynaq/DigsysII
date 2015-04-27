#include "I2Cdev.h"
#include <math.h>
#include <PID.hpp>
#include "bluetoothserial.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "MPL3115A2.h"
#include "filter.hpp"
#include <Servo.h>
#include "Wire.h"

#define DEBUG
#define MOTOR_FRONT_PIN 10 //M1
#define MOTOR_RIGHT_PIN 9  //m2
#define MOTOR_BACK_PIN 11 //M3
#define MOTOR_LEFT_PIN 6
#define ESC_INIT_DELAY 500
#define ANGLE_TO_PWM(x)    (103.0/10*x + 544)  
#define ESC_MIN 30
#define ESC_MIN_PWM 853
#define ESC_MAX 180
#define ESC_MAX_PWM 2000
#define ESC_MIN_VELOCITY ANGLE_TO_PWM(60)
#define ESC_MAX_VELOCITY ANGLE_TO_PWM(100)
#define MPU_AX_OFFSET -46
#define MPU_AY_OFFSET 1101
#define MPU_AZ_OFFSET 2148
#define MPU_GX_OFFSET 34
#define MPU_GY_OFFSET -49
#define MPU_GZ_OFFSET 9
#define MPU_CALIB_TIME 30000


bool dmpReady = false;  // set true if DMP init was successful
volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer
Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector



/*
PID constants & values
*/

float YAW_OUT;
float YAW_SETPOINT;
float YAW_KP = 0.10;
float YAW_KI = 0.0001;
float YAW_KD = 0.65;

float PITCH_OUT;
float PITCH_SETPOINT;
float PITCH_KP = 0.10;//0.18
float PITCH_KI = 0.0001;//0.0;
float PITCH_KD = 0.65;//0.55;

float ROLL_OUT;
float ROLL_SETPOINT;
float ROLL_KP = 0.1; //0.1
float ROLL_KI = 0.0001; //0.0
float ROLL_KD = 0.65; //0.75

float THROTTLE_OUT;
float ALTITUDE_SETPOINT;
float THROTTLE_KP = 0.10;
float THROTTLE_KI = 0.0001;
float THROTTLE_KD = 0.65;

float altitude;
float altitude_unfiltered;
int velocity = ANGLE_TO_PWM(95);
long dt = 100;
bool inFlight = false;



PID* yaw; PID* pitch; PID* roll; PID* throttle;
Servo front,right,back,left;
MPU6050 mpu;
MPL3115A2 dps;
Filter filter(&altitude, &altitude_unfiltered);

void setup() {
    Serial.begin(115200);
    initDPS();
    initESCs();
    initMPU();
    initPID();
}

void loop() {
    if(millis()>MPU_CALIB_TIME) inFlight = true;
    updateMPU();
    updateDPS();
    updatePID();
    updateESC();
}

void initDPS(){
  dps.begin();
  dps.setModeAltimeter();
  dps.setOversampleRate(6); // Set Oversample to the recommended 128
  dps.enableEventFlags(); // Enable all three pressure and temp event flags
}

void updateDPS(){
  altitude_unfiltered = dps.readAltitude();
  filter.update();
}


void dmpDataReady() {
    mpuInterrupt = true;
}



void initESCs(){
    front.attach(MOTOR_FRONT_PIN);
    right.attach(MOTOR_RIGHT_PIN);
    back.attach(MOTOR_BACK_PIN);
    left.attach(MOTOR_LEFT_PIN);

    #ifdef DEBUG
        Serial.print("Calibrating the servors ");
        Serial.print(ESC_MIN);
        Serial.print(" is the minimum value. ");
        Serial.print(ESC_MAX);
        Serial.println(" is the maximum value.");
    #endif

    front.writeMicroseconds(ESC_MIN_PWM);
    right.writeMicroseconds(ESC_MIN_PWM);
    back.writeMicroseconds(ESC_MIN_PWM);
    left.writeMicroseconds(ESC_MIN_PWM);
    Serial.println("Done calibrating ESCs");
    /*
    Wait a little while, else we get bus collisions if we jump straight to
    the MPU
    */
    delay(ESC_INIT_DELAY);
}


void initMPU(){
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        TWBR = 24;
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    #ifdef DEBUG
        Serial.begin(115200);
        Serial.println(F("Initializing I2C devices..."));
    #endif
    mpu.initialize();

    #ifdef DEBUG
        Serial.println(F("Testing device connections..."));
        Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") 
            : F("MPU6050 connection failed"));

        Serial.println(F("Initializing DMP..."));
    #endif

    devStatus = mpu.dmpInitialize();


  
    

    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        #ifdef DEBUG
            Serial.println(F("Enabling DMP..."));
        #endif
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        #ifdef DEBUG
            Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        #endif 

        attachInterrupt(0, dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        #ifdef DEBUG
            Serial.println(F("DMP ready! Waiting for first interrupt..."));
        #endif 
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        #ifdef DEBUG
            Serial.print(F("DMP Initialization failed (code "));
            Serial.print(devStatus);
            Serial.println(F(")"));
        #endif
    }

    /*
    Calibrate the MPU, it needs to run for about 30 seconds in our application
    Before it settles down, it starts off with big errors it needs time to
    smooth out
    */
    mpu.setXAccelOffset(MPU_AX_OFFSET);
    mpu.setYAccelOffset(MPU_AY_OFFSET);
    mpu.setZAccelOffset(MPU_AZ_OFFSET);
    
    mpu.setXGyroOffset(MPU_GX_OFFSET);
    mpu.setYGyroOffset(MPU_GY_OFFSET);
    mpu.setZGyroOffset(MPU_GZ_OFFSET);

    long timer = millis();
    while(millis()-timer < MPU_CALIB_TIME){
      updateMPU();
      updateDPS();
    }
}




/*
Function is called after the MPU is calibrated, so at this time
ypr[0] = yaw setpoint
ypr[1] = pitch setpoint
ypr[2] = roll setpoint
*/
void initPID(){
    YAW_SETPOINT = 0.0;//ypr[0];
    PITCH_SETPOINT = 0.0;//ypr[1];
    ROLL_SETPOINT = ypr[2];
    ALTITUDE_SETPOINT = 1.0; //1 meter


    yaw = new PID(&YAW_OUT, &ypr[0], &YAW_SETPOINT, dt, YAW_KP, YAW_KI, YAW_KD);
    pitch = new PID(&PITCH_OUT, &ypr[1], &PITCH_SETPOINT, dt, PITCH_KP, PITCH_KI, PITCH_KD);
    roll = new PID(&ROLL_OUT, &ypr[2], &ROLL_SETPOINT, dt, ROLL_KP, ROLL_KI, ROLL_KD);
    throttle = new PID(&THROTTLE_OUT, &altitude, &ALTITUDE_SETPOINT, dt, THROTTLE_KP, THROTTLE_KI, THROTTLE_KD);

    yaw->init();
    pitch->init();
    roll->init();
    throttle->init();
}

void updatePID(){
    if(!inFlight) return;   
    yaw->update();
    pitch->update();
    roll->update();
    throttle->update();
}

void updateESC(){
    float front_new = velocity * (1 + PITCH_OUT/100.0  + YAW_OUT/100.0);
    float back_new = velocity * (1 - PITCH_OUT/100.0 + YAW_OUT/100.0);
    float left_new = velocity * (1 + ROLL_OUT/100.0 -YAW_OUT/100.0);
    float right_new = velocity *(1 -ROLL_OUT/100.0 -YAW_OUT/100.0);
   
    if(front_new<ESC_MIN_VELOCITY) front_new = ESC_MIN_VELOCITY;
    if(front_new>ESC_MAX_VELOCITY) front_new = ESC_MAX_VELOCITY;
    if(back_new<ESC_MIN_VELOCITY) back_new = ESC_MIN_VELOCITY;
    if(back_new>ESC_MAX_VELOCITY) back_new = ESC_MAX_VELOCITY;

    if(left_new<ESC_MIN_VELOCITY) left_new = ESC_MIN_VELOCITY;
    if(left_new>ESC_MAX_VELOCITY) left_new = ESC_MAX_VELOCITY;
    if(right_new<ESC_MIN_VELOCITY) right_new = ESC_MIN_VELOCITY;
    if(right_new>ESC_MAX_VELOCITY) right_new = ESC_MAX_VELOCITY;    
   
    //float right_new = 80 + 10*(- ROLL_OUT + YAW_OUT);
    //float left_new = 80 + 10*(ROLL_OUT + YAW_OUT);
    
    
    
    
    
    
    
    if(millis()>35000){    
      //front.writeMicroseconds(front_new);
      //back.writeMicroseconds(back_new);
      //left.writeMicroseconds(left_new);
      //right.writeMicroseconds(right_new);
    }
    
    
    
    Serial.print("f,r,b,l: ");
    Serial.print(front_new, 5); Serial.print(",");
    Serial.print(right_new, 5); Serial.print(",");
    Serial.print(back_new, 5); Serial.print(",");
    Serial.println(left_new, 5);
    
}


void updateMPU(){
    if (!dmpReady) return;
    while (!mpuInterrupt && fifoCount < packetSize) {
    }
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
        #ifdef DEBUG
            Serial.println(F("FIFO overflow!"));
        #endif

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & 0x02) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;
        
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
        
        
        ypr[0] = ypr[0]/3.141592 * 180;//from radians to degrees;
        ypr[1] = ypr[1]/3.141592 * 180;
        ypr[2] = ypr[2]/3.141592 * 180;
    }
}
