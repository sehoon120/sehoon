#include "MPU9250.h"
#include <math.h>
#include <PID_v1.h>
#include <SimpleKalmanFilter.h>
//#include <Servo.h>
#include "ESC.h"

#define servo1 2 //pin number
#define servo2 3
#define servo3 4
#define servo4 5


int k = 1000;  //PID 계수
//Servo servo_1, servo_2, servo_3, servo_4;
int m_speed = 1000, M_speed = 2200;
ESC servo_1 (servo1, 800, M_speed, 800);
ESC servo_2 (servo2, 800, M_speed, 800);
ESC servo_3 (servo3, 800, M_speed, 800);
ESC servo_4 (servo4, 800, M_speed, 800);


SimpleKalmanFilter kalmanx(0.0005, 0.0005, 0.01);
SimpleKalmanFilter kalmany(0.0005, 0.0005, 0.01);

MPU9250 mpu;

int cnt = 0;
int T = 100;
//float n = 0, ty = 0, avy = 0, ooy = 0, ery = 0; // test
double outputx = 0, outputy = 0, Inputx, Inputy;  //Setpoint, Input, Output, 
double Setpointx = 0, Setpointy = 0;

PID myPIDx(&Inputx, &outputx, &Setpointx, 5, 0.7, 0.135, DIRECT);
PID myPIDy(&Inputy, &outputy, &Setpointy, 5, 0.7, 0.135, DIRECT);

float  x, y, z; //ax, ay, az,gx, gy, gz, fx, fy, fz, xx, yy, zz, ffx, ffy

float PID_X_1, PID_X_2, PID_Y_1, PID_Y_2;
float total_X = 0, total_Y = 0;//, last_X = 0, last_Y = 0;


void setup() {
    Serial.begin(115200);
    Wire.begin();
    delay(2000);

     myPIDx.SetMode(AUTOMATIC);
     myPIDy.SetMode(AUTOMATIC);
     myPIDx.SetOutputLimits(-255, +255);
     myPIDy.SetOutputLimits(-255, +255);

    if (!mpu.setup(0x68)) { 
        while (1) {
            Serial.println("MPU connection failed.");
            delay(5000);
        }
    }
    else{
      servo_1.setCalibrationDelay(1000);
      servo_2.setCalibrationDelay(1000);
      servo_3.setCalibrationDelay(1000);
      servo_4.setCalibrationDelay(1000);
      servo_1.calib();
      servo_2.calib();
      servo_3.calib();
      servo_4.calib();
      //pinMode(2, INPUT);
      //pinMode(3, INPUT);
      //pinMode(4, INPUT);
      //pinMode(5, INPUT);
      
      //servo_1.calib();
      servo_1.arm();
      servo_1.stop();
      //servo_2.calib();
      servo_2.arm();
      servo_2.stop();
      //servo_3.calib();
      servo_3.arm();
      servo_3.stop();
      //servo_4.calib();
      servo_4.arm();
      servo_4.stop();

      delay(1000);

      //servoset();
      
      
    }
    //mpu.calibrateAccelGyro();
    
}


//float tmpx = 0, tmpy = 0, tmpz = 0;
//int cn = 0;
void loop() {
  //cn = cn + 1;
  mpu.update();
  x = mpu.getAccX()-0.06;
  y = mpu.getAccY()+0.03;
  z = mpu.getAccZ()-1.01;
  //tmpx = tmpx + x;
  //tmpy = tmpy + y;
  //tmpz = tmpz + z;

  x = (abs(x) > 8/2000 ? x : 0);
  y = (abs(y) > 14/2000 ? y : 0);
  z = (abs(z) > 10/2000 ? z : 0);
  
  /*Serial.println(x);
  Serial.println(y);
  Serial.println(z);
  Serial.println();*/

  getPID_X();
  getPID_Y();
  
  //ign or control or avg
  //ign();
  servocontrol();
  //avg();

  
  delay(T);
}
/*
void ign(){
  //x 0.6, y 0.6  /2000(k)
  if (abs(outputx - last_X) <= 0.6 / k){
    outputx = last_X;
    
    
  }
  Serial.print("X : ");
  Serial.println(-outputx * k);
  last_X = outputx;
  
  if (abs(outputy - last_Y) <= 0.6 / k){
    outputy = last_Y;
  }
  Serial.print("Y : ");
  Serial.println(-outputy * k);
  last_Y = PID_Y_1;
  servocontrol();
}*/
void avg (){
  total_X = total_X + PID_X_1;
  total_Y = total_Y + PID_Y_1;
  cnt = cnt + 1;
  if(cnt == 5){
    PID_X_1 = total_X / 5;
    PID_X_2 = -PID_X_1;
    PID_Y_1 = total_Y / 5;
    PID_Y_2 = -PID_Y_1;
  } 
  servocontrol();
  if(cnt == 5){
    /*Serial.print("X : ");
    Serial.println(PID_X_1);
    Serial.print("Y : ");
    Serial.println(PID_Y_1);*/
    total_X = 0;
    total_Y = 0;
    cnt = 0;
  }
}


void getPID_X(void){
  float ax = atan(x/sqrt(y*y+z*z));
  float tmpx = mpu.getGyroX() - 2.43;
  float xx = 0;
  xx = xx + (abs(tmpx) > 0.185 ? tmpx : 0) *T/1000;
  //gx = xx/180*PI;
  //fx = 0.98 * xx / 180 * PI + 0.02 * ax;
  float ffx = kalmanx.updateEstimate(0.98 * xx / 180 * PI + 0.02 * ax);//kalmanX
  Inputx = ffx;
  myPIDx.Compute();

  PID_X_1 = -outputx*k;
  PID_X_2 = outputx*k;
  Serial.print("PID X1:");
  Serial.println(PID_X_1);
  //Serial.print("PID X2:");
  //Serial.println(PID_X_2);
  //PID_X_1 = constrain(PID_X_1, m_speed, M_speed);
  //PID_X_2 = constrain(PID_X_2, m_speed, M_speed);
  
}

void getPID_Y(void){
  float ay = atan(y/sqrt(x*x+z*z));
  float tmpy = mpu.getGyroY() - 0.49;
  float yy = 0;
  yy = yy + (abs(tmpy) > 0.17 ? tmpy : 0) *T/1000;
  //gy = (yy)/180*PI;
  //fy = 0.98 * yy / 180 * PI + 0.02 * ay;
  float ffy = kalmany.updateEstimate(0.98 * yy / 180 * PI + 0.02 * ay);//kalmanY
  Inputy = ffy;
  myPIDy.Compute();
  
  PID_Y_1 = -outputy*k;
  PID_Y_2 = outputy*k;
  Serial.print("PID Y1:");
  Serial.println(PID_Y_1);
  //Serial.print("PID Y2:");
  //Serial.println(PID_Y_2);
  //PID_Y_1 = constrain(PID_Y_1, m_speed, M_speed);
  //PID_Y_2 = constrain(PID_Y_2, m_speed, M_speed);
}

/*
void servoset(){
  servo_1.speed(m_speed);
  servo_2.speed(m_speed);
  servo_3.speed(m_speed);
  servo_4.speed(m_speed);
  
}*/

void servocontrol(){
  float stand = 100;
  
  if(PID_X_1 > stand){
    if(PID_Y_1 >stand){
      servo_1.speed(PID_X_1 + PID_Y_1 + m_speed);
      servo_4.speed(m_speed + PID_X_1);
      servo_2.speed(m_speed + PID_Y_1);
      servo_3.speed(m_speed);
      
    }
    else if(PID_Y_2 >stand){
      servo_4.speed(PID_X_1 + PID_Y_2 + m_speed);
      servo_1.speed(PID_X_1 + m_speed);
      servo_3.speed(PID_Y_2 + m_speed);
      servo_2.speed(m_speed);
    }
    else{
      servo_1.speed(PID_X_1 + m_speed);
      servo_4.speed(PID_X_1 + m_speed);
      servo_2.speed(m_speed);
      servo_3.speed(m_speed);
      
    }
  }
  else if(PID_X_2 >stand){
    if(PID_Y_1 > m_speed + stand){
      servo_2.speed(PID_X_2 + PID_Y_1 + m_speed);
      servo_3.speed(PID_X_2+ m_speed);
      servo_1.speed(PID_Y_1 + m_speed);
      servo_4.speed(m_speed);
    }
    else if(PID_Y_2 >stand){
      servo_3.speed(PID_X_2 + PID_Y_2 + m_speed);
      servo_2.speed(PID_X_2 + m_speed);
      servo_4.speed(PID_Y_2 + m_speed);
      servo_1.speed(m_speed);
    }
    else{
      servo_2.speed(PID_X_2 + m_speed);
      servo_3.speed(PID_X_2 + m_speed);
      servo_1.speed(m_speed);
      servo_4.speed(m_speed);
      
    }
  }
  else{
    if(PID_Y_1 >stand){
      servo_1.speed(PID_Y_1 + m_speed);
      servo_2.speed(PID_Y_1 + m_speed);
      servo_3.speed(m_speed);
      servo_4.speed(m_speed);
    }
    else if(PID_Y_2 >stand){
      servo_3.speed(PID_Y_2 + m_speed);
      servo_4.speed(PID_Y_2 + m_speed);
      servo_1.speed(m_speed);
      servo_2.speed(m_speed);
    }
    else{
      servo_1.speed(m_speed);
      servo_2.speed(m_speed);
      servo_3.speed(m_speed);
      servo_4.speed(m_speed);
    }
  }
  
  
  
}

/*
void servoset(){

  servo_1.attach(servo1);
  servo_2.attach(servo2);
  servo_3.attach(servo3);
  servo_4.attach(servo4);
  
  servo_1.writeMicroseconds(m_speed);
  servo_2.writeMicroseconds(m_speed);
  servo_3.writeMicroseconds(m_speed);
  servo_4.writeMicroseconds(m_speed);
  
}*/

/*
void servocontrol(){
  //float s1 = m_speed, s2 = m_speed, s3 = m_speed, s4 = m_speed;
  float stand = 10;
  if(PID_X_1 > m_speed + stand){
    if(PID_Y_1 > m_speed + stand){
      servo_1.writeMicroseconds(PID_X_1 + PID_Y_1 - m_speed);
      servo_4.writeMicroseconds(PID_X_1);
      servo_2.writeMicroseconds(PID_Y_1);
      servo_3.writeMicroseconds(m_speed);
    }
    else if(PID_Y_2 > m_speed + stand){
      servo_4.writeMicroseconds(PID_X_1 + PID_Y_1 - m_speed);
      servo_1.writeMicroseconds(PID_X_1);
      servo_3.writeMicroseconds(PID_Y_1);
      servo_2.writeMicroseconds(m_speed);
    }
    else{
      servo_1.writeMicroseconds(PID_X_1);
      servo_4.writeMicroseconds(PID_X_1);
      servo_2.writeMicroseconds(m_speed);
      servo_3.writeMicroseconds(m_speed);
      
    }
  }
  else if(PID_X_2 > m_speed + stand){
    if(PID_Y_1 > m_speed + stand){
      servo_2.writeMicroseconds(PID_X_1 + PID_Y_1 - m_speed);
      servo_3.writeMicroseconds(PID_X_1);
      servo_1.writeMicroseconds(PID_Y_1);
      servo_4.writeMicroseconds(m_speed);
    }
    else if(PID_Y_2 > m_speed + stand){
      servo_3.writeMicroseconds(PID_X_1 + PID_Y_1 - m_speed);
      servo_2.writeMicroseconds(PID_X_1);
      servo_4.writeMicroseconds(PID_Y_1);
      servo_1.writeMicroseconds(m_speed);
    }
    else{
      servo_2.writeMicroseconds(PID_X_1);
      servo_3.writeMicroseconds(PID_X_1);
      servo_1.writeMicroseconds(m_speed);
      servo_4.writeMicroseconds(m_speed);
      
    }
  }
  else{
    if(PID_Y_1 > m_speed + stand){
      servo_1.writeMicroseconds(PID_Y_1);
      servo_2.writeMicroseconds(PID_Y_1);
      servo_3.writeMicroseconds(m_speed);
      servo_4.writeMicroseconds(m_speed);
    }
    else if(PID_Y_2 > m_speed + stand){
      servo_3.writeMicroseconds(PID_Y_1);
      servo_4.writeMicroseconds(PID_Y_1);
      servo_1.writeMicroseconds(m_speed);
      servo_2.writeMicroseconds(m_speed);
    }
    else{
      servo_1.writeMicroseconds(m_speed);
      servo_2.writeMicroseconds(m_speed);
      servo_3.writeMicroseconds(m_speed);
      servo_4.writeMicroseconds(m_speed);
    }
    //s1 = m_speed;
    //s2 = m_speed;
    //s3 = m_speed;
    //s4 = m_speed;
  }

  
}*/
