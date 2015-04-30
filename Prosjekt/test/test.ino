#include "I2Cdev.h"
#include <math.h>
#include "PID.hpp"
#include "bluetoothserial.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "MPL3115A2.h"
#include "filter.hpp"
#include <Servo.h>
#include "Wire.h"

/*For utskrift til serial */
#define DEBUG
#define MOTOR_FRONT_PIN 10 //M1
#define MOTOR_RIGHT_PIN 9  //m2
#define MOTOR_BACK_PIN 11 //M3
#define MOTOR_LEFT_PIN 6 //M4
/*Tid før ESC er ferdig armert */
#define ESC_INIT_DELAY 500
/*
Formel for å gjøre servo vinkler (0-180) til PWM signaler for ESCene
Det er bedre å bruke PWM da det har mye større range enn vinkler.
Formelen fant ved y = a0 +bx
*/
#define ANGLE_TO_PWM(x)    (103.0/10*x + 544)
/*Minimum servo angle for å amrere ESCen */
#define ESC_MIN 30
/*Det samme i PWM */
#define ESC_MIN_PWM 853
/*Maximum ESC angle, 180 grader */
#define ESC_MAX 180
/*Det samme i PWM */
#define ESC_MAX_PWM 2000
/*Minste PWM for rotasjon på ESC */
#define ESC_MIN_VELOCITY ANGLE_TO_PWM(60)
/*Det maksimale vi tillatter ESCen vår */
#define ESC_MAX_VELOCITY ANGLE_TO_PWM(90)
/*Hvor mange grader/pwm vi kan øke hastigheten med per trykk på knapp
fra bluetooth-app */
#define ESC_VELOCITY_INCREMENT ANGLE_TO_PWM(0.25);
/*Disse offsettene er IKKE fabrikkinnstilt. De varier fra MPU til MPU.
Vi kjørte en sketch fra http://www.i2cdevlib.com/forums/topic/96-arduino-sketch-to-automatically-calculate-mpu6050-offsets/
Det er viktig at MPUen kalibreres med disse offsettene, ellers er outputs fra sensoren unøyaktige
*/
#define MPU_AX_OFFSET -46
#define MPU_AY_OFFSET 1101
#define MPU_AZ_OFFSET 2148
#define MPU_GX_OFFSET 34
#define MPU_GY_OFFSET -49
#define MPU_GZ_OFFSET 9
/*
MPUen har en DMP (digital motion processor) innebygget. Den bruker ca 30s før
den er kalibrert. Dette er en intern process på chipen til MPUen, vi må bare vente.
*/
#define MPU_CALIB_TIME 30000
/*
Alt er satt opp etter så lang tid. Da er vi klar for flight.
*/
#define QUADCOPTER_ARM_TIME 35000
/*Bluetooth variabler */
#define BT_RX_PIN 8
#define BT_TX_PIN 7
/*Maksimale endringer vi kan gjøre på vinkler fra bluetooth appen.
Disse vil feedes inni PIDen vår.
*/
#define BT_MAX_YAW_CHANGE 10
#define BT_MAX_PITCH_CHANGE 10
#define BT_MAX_ROLL_CHANGE 10
/*
Hvor mye hver vinkel kan økes med per tastetrykk fra appen.
*/
#define YAW_INCREMENT 0.5
#define PITCH_INCREMENT 0.5
#define ROLL_INCREMENT 0.5

/*Bool for å sjekke om DMPen er klar til bruk */
bool dmpReady = false;
/*Sjekker om MPUen har fått interrupt */
volatile bool mpuInterrupt = false;
/*Intern MPU interrupt byte. 0x10 = overflow på data, 0x2, klar */
uint8_t mpuIntStatus;
/*Holder statusen til MPUen. Hvis 0 = in operatior. Else error */
uint8_t devStatus;
/*Størrelse på datapacketen fra MPUen sin DMP, i bytes, default 42 bytes */
uint16_t packetSize;
/*Antall bytes som er klar for innehenting på DMPen sin FIFO (first in, first out, buffer) */
uint16_t fifoCount;
/*Bufferen til DMPen sin data */
uint8_t fifoBuffer[64];
/*Rotasjonstilstanden til MPUen. Quaternion fordi det er ikke
utsatt for gimbal-lock. Gimbalock fenomen som oppstår hvis man jobber direkte med
yaw pitch og roll (fra sensor reading til neste sensorreading). Hvis readingene er direkte
yaw pitch og roll så vil de over tid føre til gimbal lokc. Gimbal lock = 2 akser låses sammen.
Du kan ikke endre yaw uten å endre pitch f.eks
*/
Quaternion q;
/*Gravitasjonsvektoren x,y,z Brukes og oppdateres av MPUen */
VectorFloat gravity;
/*Holder yaw pitch og roll verdiene */
float ypr[3];



/*====YAW VARIABLER======*/

/*Outputen til yaw piden. Dette er ikke en vinkel men en feeedback korreksjon fra PIDen */
float YAW_OUT;
/*Den yaw-vinkelen PIDen skal jobbe for å vedlikeholde */
float YAW_SETPOINT;
/*Yaw-vinkelen quadcopteren starter med når programmet starter (da quadcopteren er horisontal) */
float YAW_ORIG_SETPOINT;
/*PID verdier, disse er ikke lett å justere...eksperiment oppå eksperiment */
float YAW_KP = 0.10;
float YAW_KI = 0.0001;
float YAW_KD = 0.65;

/*====PITCH VARIABLER=====*/
float PITCH_OUT;
float PITCH_SETPOINT;
float PITCH_ORIG_SETPOINT;
float PITCH_KP = 0.10;//0.18
float PITCH_KI = 0.0002;//0.0;
float PITCH_KD = 0.65;//0.55;

/*====ROLL VARIABLER=====*/
float ROLL_OUT;
float ROLL_SETPOINT;
float ROLL_ORIG_SETPOINT;
float ROLL_KP = 0.1; //0.1
float ROLL_KI = 0.0002; //0.0
float ROLL_KD = 0.65; //0.75

/*====THROTTLE VARIABLER=====*/
float THROTTLE_OUT;
/*Vi har en DPS (digital pressure sensor) som vi kan feede til en altitude PID for å vedlikeholde høyde.
Dette bruker vi ikke foreløpig. Mest fordi sensoren har en unøyaktiget på 0.3 meter. Se filter lenger ned
*/
float ALTITUDE_SETPOINT;
float ALTITUDE_ORIG_SETPOINT;
float THROTTLE_KP = 0.10;
float THROTTLE_KI = 0.0001;
float THROTTLE_KD = 0.65;

float altitude = 0;
float altitude_unfiltered = 0;
float velocity = ESC_MIN_VELOCITY;
float velocity_prev = ESC_MIN_VELOCITY;


/*Tidssteg mellom hver pid oppdatering, i millisekunder */
int dt = 100;
/*Blir true når millis() = QUADCOPTER_ARM_TIME, da er vi klar til å fly */
bool inFlight = false;


/*PID pointers, de krever å starte med verdier som ikke er satt enda. Så vi er nødt til å
bruke new/malloc senere, kan ikke lage PID objektene her */
PID* yaw; PID* pitch; PID* roll; PID* throttle;
/*ESCene */
Servo front,right,back,left;
/*MPUen, veldig fin bibliotek */
MPU6050 mpu;
/*Digital Pressure Sensor, ikke så fin...Her var vi nødt til å fjerne delay fra bibliotekets
readAltitude() metode, en delay fører til at MPUen sin FIFO ikke leses kjapt nok. Fører til overflow
*/
MPL3115A2 dps;
/*Digital lavpass filter vi selv har skrevet. Med en alpha = 0.05. 
Den er veldig aggressiv, men det er fordi det er hopp på 0.3-1.0 meter på sensoroutput fra
trykksensoren.
*/
Filter filter(&altitude, &altitude_unfiltered);
/*Bluetoothen fungerer som en serial connection mot arduinoen */
SoftwareSerial bt(BT_TX_PIN, BT_RX_PIN);

void setup() {
    Serial.begin(115200);
    bt.begin(9600);
    initDPS();
    initESCs();
    initMPU();
    initPID();
}

void loop() {
    if(millis()>QUADCOPTER_ARM_TIME) inFlight = true;
    
    updateMPU();
    updateDPS();
    updateBT();
    updatePID();
    updateESC();
}

/*
Funksjonen sjekker om det har kommet noe data til bluetoothen.
Hvis ny data:

data = 0, øk hastighet
data = 1, senk hastighet
data = 2, øk yaw_setpoint
data = 3, senk yaw_setpoint
data = 4, øk pitch_setpoint
data = 5, senk pitch_setpoint
data = 6, øk roll_setpoint
data = 7, senk roll_setpoint
*/
void updateBT(){
  if(!(bt.available())) return;
  
  byte in = bt.read();
  
  /*Velocity update */
  if((int)in == 0){
    velocity += ESC_VELOCITY_INCREMENT;   
  }
  if((int)in == 1){
    velocity -= ESC_VELOCITY_INCREMENT;
  }
  
  /*Yaw update */
  if((int)in == 2){
    if(YAW_SETPOINT < BT_MAX_YAW_CHANGE)
      YAW_SETPOINT += YAW_INCREMENT;
  }
  if((int)in == 3){
    if(YAW_SETPOINT > -BT_MAX_YAW_CHANGE)
      YAW_SETPOINT -= YAW_INCREMENT;
  }
  
  /*Pitch update */
  if((int)in == 4){
    if(PITCH_SETPOINT < BT_MAX_PITCH_CHANGE)
      PITCH_SETPOINT += PITCH_INCREMENT;
  }
  if((int)in == 5){
    if(PITCH_SETPOINT > -BT_MAX_PITCH_CHANGE)
      PITCH_SETPOINT -= PITCH_INCREMENT;
  }
  
  /*Roll update */
  if((int)in == 6){
    if(ROLL_SETPOINT < BT_MAX_ROLL_CHANGE)
      ROLL_SETPOINT += ROLL_INCREMENT;
  }
  if((int)in == 7){
    if(ROLL_SETPOINT >-BT_MAX_ROLL_CHANGE)
      ROLL_SETPOINT -= ROLL_INCREMENT;
  }
}

/*
Starter Digital Pressure Sensor.
Setter den til å måle høyde (istedenfor trykk)
Setter den til å sample mer enn det den trenger (mer realtime)
Setter den til å enable måling av trykk og temperature
*/
void initDPS(){
  dps.begin();
  dps.setModeAltimeter();
  dps.setOversampleRate(6);
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
  /*Begynn I2C kommnunikasjon */
  Wire.begin();
  /*Setter SCL clock speed. 12 = 200Khz. Dette gjør det lettere å lese av 
  data fra MPUen sin FIFO. Med 2 sensorer på I2C-en og mye beregninger så kan
  CPU clocks mellom hver FIFO dump til arduino fra MPUen være høyere enn det MPUen selv
  setter i sin egen FIFO. Dette fører til FIFO overflow og sensordata utilregnelig.
  */
  TWBR = 12;
  Serial.begin(115200);
  Serial.println(F("Initializing I2C devices..."));
  mpu.initialize();
  Serial.println(F("Testing device connections..."));
  Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") 
            : F("MPU6050 connection failed"));
  Serial.println(F("Initializing DMP..."));
  devStatus = mpu.dmpInitialize();
  // make sure it worked (returns 0 if so)
  if (devStatus == 0) {
    Serial.println(F("Enabling DMP..."));
    mpu.setDMPEnabled(true);
    Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
    attachInterrupt(0, dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();
    Serial.println(F("DMP ready! Waiting for first interrupt..."));
    dmpReady = true;
    packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
      Serial.print(F("DMP Initialization failed (code "));
      Serial.print(devStatus);
      Serial.println(F(")"));
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
    YAW_SETPOINT = ypr[0];
    PITCH_SETPOINT = ypr[1];
    ROLL_SETPOINT = ypr[2];
    ALTITUDE_SETPOINT = 1.0;
    
    
    YAW_ORIG_SETPOINT = YAW_SETPOINT;
    PITCH_ORIG_SETPOINT = PITCH_SETPOINT;
    ROLL_ORIG_SETPOINT= ROLL_SETPOINT;


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
  
  float front_new = velocity * ((1+PITCH_OUT/100.0));
  float back_new = velocity * ((1-PITCH_OUT/100.0));
  float left_new = velocity * ((1+ROLL_OUT/100.0)); 
  float right_new = velocity * ((1-ROLL_OUT/100.0)); 
  /*
    float front_new = (velocity * (1 + PITCH_OUT/100.0));//  + YAW_OUT/100.0);
    float back_new = (velocity * (1 - PITCH_OUT/100.0));// + YAW_OUT/100.0);
    float left_new = (velocity* (1 + ROLL_OUT/100.0));//-YAW_OUT/100.0);
    float right_new = (velocity *(1 -ROLL_OUT/100.0));// -YAW_OUT/100.0);
   */
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
    
    
    
    
    
    /*
    if(millis()>35000){  
    
      front.writeMicroseconds(front_new);
      back.writeMicroseconds(back_new);
      left.writeMicroseconds(left_new);
      right.writeMicroseconds(right_new);
      
      
    }
    */
    
    
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
