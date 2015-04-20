#include "I2Cdev.h"
#include <math.h>
#include "PID.hpp"
#include "MPU6050_6Axis_MotionApps20.h"
#include <Servo.h>
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

#define DEBUG

/*
Motor constants & values
*/
Servo front,right,back,left;
#define MOTOR_FRONT_PIN 6 
#define MOTOR_RIGHT_PIN 9
#define MOTOR_BACK_PIN 10
#define MOTOR_LEFT_PIN 11
#define ESC_INIT_DELAY 500

/*
MPU constants and values 
*/
MPU6050 mpu;
#define MPU_CALIB_TIME 30000

bool dmpReady = false;  // set true if DMP init was successful
volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector



/*
PID constants & values
*/
PID yaw; PID pitch; PID roll;

float YAW_OUT;
float YAW_SETPOINT;
float YAW_KP = 1.0;
float YAW_KD = 0.05;
float YAW_KI = 0.1;

float PITCH_OUT;
float PITCH_SETPOINT;
float PITCH_KP = 1.0;
float PITCH_KD = 0.05;
float PITCH_KI = 0.1;

float ROLL_OUT;
float ROLL_SETPOINT;
float ROLL_KP = 1.0;
float ROLL_KD = 0.05;
float ROLL_KI = 01;

float dt = 0.1;





void setup() {
    initESCs();
    initMPU();
    initPID();
}

void loop() {
    updateMPU();
    updatePID();
    updateESC();
}


void dmpDataReady() {
    mpuInterrupt = true;
}



void initESCs(){
    front.attach(MOTOR_FRONT_PIN);
    right.attach(MOTOR_RIGHT_PIN)
    back.attach(MOTOR_BACK_PIN);
    left.attach(MOTOR_LEFT_PIN);

    #if DEBUG =={
        Serial.print("Calibrating the servors ");
        Serial.print(ESC_MIN);
        Serial.print(" is the minimum value. ");
        Serial.print(ESC_frontX);
        Serial.println(" is the maximum value.")
    }

    front.write(ESC_MIN);
    right.write(ESC_MIN);
    back.write(ESC_MIN);
    left.write(ESC_MIN);
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

    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

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

    long timer = millis();
    while(millis()-timer < MPU_CALIB_TIME) updateMPU();
}




/*
Function is called after the MPU is calibrated, so at this time
ypr[0] = yaw setpoint
ypr[1] = pitch setpoint
ypr[2] = roll setpoint
*/
void initPID(){
    *YAW_SETPOINT = ypr[0];
    *PITCH_SETPOINT = ypr[1];
    *ROLL_SETPOINT = ypr[2];


    yaw = PID(&YAW_OUT, &ypr[0], &YAW_SETPOINT, dt, YAW_KP, YAW_KI, YAW_KD);
    pitch = PID(&PITCH_OUT, &ypr[1], &PITCH_SETPOINT, dt, PITCH_KP, PITCH_KI, PITCH_KD);
    roll = PID(&ROLL_OUT, &ypr[2], &ROLL_SETPOINT, dt, ROLL_KP, ROLL_KI, ROLL_KD);

    yaw.init();
    pitch.init();
    roll.init();
}

void updatePID(){
    yaw.update();
    pitch.update();
    roll.update();
}

void updateESC(){
    float front_new = PITCH_OUT - YAW_OUT;
    float back_new = -PITCH_OUT - YAW_OUT;

    float right_new = - ROLL_OUT + YAW_OUT;
    float left_new = ROLL_OUT + YAW_OUT;


    //front.write(front_new);
    //back.write(back_new);
    //left.write(right_new);
    //right.write(left_new);


    Serial.print("y,p,c,f,b,l,r: ");
    Serial.print(YAW_OUT); Serial.print(",");
    Serial.print(PITCH_OUT); Serial.print(",");
    Serial.print(ROLL_OUT); Serial.print(",");
    Serial.print(front_new); Serial.print(",");
    Serial.print(back_new); Serial.print(",");
    Serial.print(left_new); Serial.print(",");
    Serial.println(right_new);      
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
    }
}