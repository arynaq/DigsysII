#include "I2Cdev.h"
#include <math.h>
#include "PID.hpp"
#include "MPU6050_6Axis_MotionApps20.h"
#include <Servo.h>
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif



void initMPU(void);
void initESCs(void);
void initPID(void);

MPU6050 mpu;

//#define OUTPUT_READABLE_QUATERNION
//#define OUTPUT_READABLE_EULER
#define OUTPUT_READABLE_YAWPITCHROLL
//#define OUTPUT_READABLE_REALACCEL
//#define OUTPUT_READABLE_WORLDACCEL
//#define OUTPUT_TEAPOT


// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

// packet structure for InvenSense teapot demo
uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };



// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}


//PIDS
float yaw_0;
float pitch_0;
float roll_0;

PID yaw = PID(&yaw_0, &ypr[0], 0.1);
PID pitch = PID(&pitch_0, &ypr[1], 0.1);
PID roll = PID(&roll_0, &ypr[2], 0.1);

bool pidsInitialized = false;





//Motors
boolean calibrated = false;
Servo m1,m2,m3,m4;


void setup() {
           
    
    //Setup motors
    m1.attach(6);
    m2.attach(9);
    m3.attach(10);
    m4.attach(11);
    Serial.println("Calibrating the servos, 30 is min, 180 is max");
    m1.write(30);
    m2.write(30);
    m3.write(30);
    m4.write(30);
    Serial.println("Calibration done");    
    delay(500);

    
  
  
    
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        TWBR = 24;
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif
    
    Serial.begin(115200);
    Serial.println(F("Initializing I2C devices..."));
    
    mpu.initialize();

    Serial.println(F("Testing device connections..."));
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

    // load and configure the DMP
    Serial.println(F("Initializing DMP..."));
    devStatus = mpu.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        attachInterrupt(0, dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }

    
    
    
}

void loop() {
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
        Serial.println(F("FIFO overflow!"));

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & 0x02) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;
        
        
        
         
        
        
          
        #ifdef OUTPUT_READABLE_YAWPITCHROLL
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
            
         
         
          
          if(millis()<30000){
            Serial.print("Waiting for stabilization, ");
            Serial.print(0.001* (30000-millis()));
            Serial.println("seconds left.");
            return;
          }
          
          if(pidsInitialized == false){
            yaw_0 =  ypr[0];
            pitch_0 = ypr[1];
            roll_0 = ypr[2];
            
            pidsInitialized = true;
         }
         
         
          yaw.update();
          pitch.update();
          roll.update();
        
         
          
          
          
          
           
          float c_yaw = yaw.out();
          float c_pitch = pitch.out();
          float c_roll =  roll.out();
          
          
          float motor1 = 65 - c_pitch - c_yaw;   //rear
          float motor2 = 65 + c_roll  + c_yaw;   //left
          float motor3 = 65 + c_pitch - c_yaw;   //front 
          float motor4 = 65 - c_roll  + c_yaw;   //right
          
          
          Serial.print("y,p,c,m1,m2,m3,m4: ");
          Serial.print(c_yaw); Serial.print(",");
          Serial.print(c_pitch); Serial.print(",");
          Serial.print(c_roll); Serial.print(",");
          Serial.print(motor1); Serial.print(",");
          Serial.print(motor2); Serial.print(",");
          Serial.print(motor3); Serial.print(",");
          Serial.println(motor4);
          
          
          m1.write(motor1);
          m2.write(motor2);
          m3.write(motor3);
          m4.write(motor4);
         
          
          
          
          /*
          
          if(motor1>=90) motor1 = 90;
          if(motor1<=60) motor1 = 60;
          
          if(motor2>=90) motor2 = 90;
          if(motor1<=60) motor2 = 60;
          
          if(motor3>=90) motor3 = 90;
          if(motor3<=60) motor3 = 60;
          
          if(motor4>=90) motor4 = 90;
          if(motor4<=60) motor4 = 60;
          
          */
       
        #endif
        
        

        #ifdef OUTPUT_READABLE_QUATERNION
            // display quaternion values in easy matrix form: w x y z
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            
            
            //Serial.print("quat\t");
            //Serial.print(q.w);
            //Serial.print("\t");
            //Serial.print(q.x);
            //Serial.print("\t");
            //Serial.print(q.y);
            //Serial.print("\t");
            //Serial.println(q.z);
            //q13 = 2*(q.x*q.z - q.w*q.y);
            //q23 = 2*(q.y*q.z + q.w*q.x);
            //q33 = 1- (2*q.x*q.x) - 2*q.y*q.y;
            
            
            /*Serial.println("BEFORE");
            Serial.print("M1, M2, M3, M4: ");
            Serial.print(m1); Serial.print(",");
            Serial.print(m2); Serial.print(",");
            Serial.print(m3); Serial.print(",");
            Serial.println(m4);
            
            //m1 = m1 + 0.05*q13;
            //m2 = m2 + 0.05*q23;
            //m3 = m3 - 0.05*q13;
            //m4 = m4 - 0.05*q23;
            
            Serial.println("AFTER");
            Serial.print("M1, M2, M3, M4: ");
            Serial.print(m1); Serial.print(",");
            Serial.print(m2); Serial.print(",");
            Serial.print(m3); Serial.print(",");
            Serial.println(m4);
            */
            
            
        #endif

        #ifdef OUTPUT_READABLE_EULER
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetEuler(euler, &q);
            Serial.print("euler\t");
            Serial.print(euler[0] * 180/M_PI);
            Serial.print("\t");
            Serial.print(euler[1] * 180/M_PI);
            Serial.print("\t");
            Serial.println(euler[2] * 180/M_PI);
        #endif


        #ifdef OUTPUT_READABLE_REALACCEL
            // display real acceleration, adjusted to remove gravity
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetAccel(&aa, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
            Serial.print("areal\t");
            Serial.print(aaReal.x);
            Serial.print("\t");
            Serial.print(aaReal.y);
            Serial.print("\t");
            Serial.println(aaReal.z);
        #endif

        #ifdef OUTPUT_READABLE_WORLDACCEL
            // display initial world-frame acceleration, adjusted to remove gravity
            // and rotated based on known orientation from quaternion
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetAccel(&aa, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
            mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);
            Serial.print("aworld\t");
            Serial.print(aaWorld.x);
            Serial.print("\t");
            Serial.print(aaWorld.y);
            Serial.print("\t");
            Serial.println(aaWorld.z);
        #endif
    
        #ifdef OUTPUT_TEAPOT
            // display quaternion values in InvenSense Teapot demo format:
            teapotPacket[2] = fifoBuffer[0];
            teapotPacket[3] = fifoBuffer[1];
            teapotPacket[4] = fifoBuffer[4];
            teapotPacket[5] = fifoBuffer[5];
            teapotPacket[6] = fifoBuffer[8];
            teapotPacket[7] = fifoBuffer[9];
            teapotPacket[8] = fifoBuffer[12];
            teapotPacket[9] = fifoBuffer[13];
            Serial.write(teapotPacket, 14);
            teapotPacket[11]++; // packetCount, loops at 0xFF on purpose
        #endif
        
    }
    
      
    }
