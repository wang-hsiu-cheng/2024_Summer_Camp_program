/*
   -- v1 --

   This source code of graphical user interface
   has been generated automatically by RemoteXY editor.
   To compile this code using RemoteXY library 3.1.10 or later version
   download by link http://remotexy.com/en/library/
   To connect using RemoteXY mobile app by link http://remotexy.com/en/download/
     - for ANDROID 4.13.11 or later version;
     - for iOS 1.10.3 or later version;

   This source code is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
*/

/*
  Note: current wheel control functions haven't finished
  使用此遙控程式時，手機橫向擺放來操作
  理想上希望蘑菇頭指向哪個方向，車子就會轉向那個方向然後繼續直走
  目前是蘑菇頭指向不同方向時，車子會以不同的角速度持續走弧線(也就是以不同的曲率半徑前進)
  如果蘑菇頭指向轉盤上半圓的區域，車子會以前進的方式行走
  如果蘑菇頭指向轉盤下半圓的區域，車子會以倒退的方式行走
*/

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// you can enable debug logging to Serial at 115200
// #define REMOTEXY__DEBUGLOG

// RemoteXY select connection mode and include library
#define REMOTEXY_MODE__ESP8266WIFI_LIB_POINT

#include <ESP8266WiFi.h>

// RemoteXY connection settings
#define REMOTEXY_WIFI_SSID "RemoteXY_TEST"
#define REMOTEXY_WIFI_PASSWORD ""
#define REMOTEXY_SERVER_PORT 6377

#include <RemoteXY.h>
#include <Servo.h>
Servo myservo_00;
Servo myservo_01;
// RemoteXY GUI configuration
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] = // 45 bytes
    {255, 4, 0, 0, 0, 38, 0, 17, 0, 0, 0, 31, 1, 106, 200, 1, 1, 3, 0, 5,
     20, 121, 60, 60, 0, 177, 109, 31, 1, 38, 77, 24, 24, 0, 2, 31, 0, 4, 43, 18,
     14, 36, 0, 2, 26};

// this structure defines all the variables and events of your control interface
struct
{

  // input variables
  int8_t joystick_01_x; // from -100 to 100
  int8_t joystick_01_y; // from -100 to 100
  uint8_t button_02;    // if button pressed = 1 and kick ball, else = 0
  int8_t slider_01;     // from 0 to 100, control two servo's angle

  // other variable
  uint8_t connect_flag; // =1 if wire connected, else =0

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

static const uint8_t D0 = 16;
static const uint8_t D1 = 5;
static const uint8_t D2 = 4;
static const uint8_t D3 = 0;
static const uint8_t D4 = 2;
static const uint8_t D5 = 14;
static const uint8_t D6 = 12;
static const uint8_t D7 = 13;
static const uint8_t D8 = 15;

const int speed_A = D1;     // right wheel
const int directon_A = D4;  // right wheel
const int speed_B = D2;     // left wheel
const int directon_B = D3;  // left wheel
const int n20_motor = D5;   // shoot ball
const int servo_0 = D7;     // right servo gate
const int servo_1 = D6;     // left servo gate

bool isBackward = false;
int targetAngle;
int servoAngle = 0;
int joystick_y;
int joystick_x;
float carDirection = 0;
float carVelocity = 0;
float angularVelocity = 0;
float carWidth = 0.205;

void setup()
{
  Serial.begin(115200);
  RemoteXY_Init();
  pinMode(speed_A, OUTPUT);
  pinMode(directon_A, OUTPUT);
  pinMode(speed_B, OUTPUT);
  pinMode(directon_B, OUTPUT);
  pinMode(n20_motor, OUTPUT);
  myservo_00.attach(servo_0);
  myservo_01.attach(servo_1);
  // TODO you setup code
}

// a map function, transform the wheel velocity velue to the range we want
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}
// use current joystick value to calculate the car velocity and moving direction it should be.
void Transformation()
{
  // if joystick value small enough, set car velocity to 0
  if (abs(joystick_x) <= 10 && abs(joystick_y) <= 10)
  {
    carVelocity = 0;
    carDirection = 0;
    return;
  }
  // calculate the car velocity
  carVelocity = sqrt(joystick_y * joystick_y + joystick_x * joystick_x) / 1000;
  // determine if car have to move forward or backward. according to joystick_x velue
  if (joystick_x < 0)
    isBackward = true;
  else
    isBackward = false;
  // determine if car have to turn left or right. according to joystick_y velue
  if (joystick_y > 0)
    carDirection = atan(joystick_y / abs(joystick_x));    // turn left. this value must be positive
  else if (joystick_y <= 0)
    carDirection = -atan(-joystick_y / abs(joystick_x));  // turn right. this value must be negative
}
// convert the car velocity and direction to the wheels' velocity
void motorControl()
{
  // get the car velocity and moving direction it should be
  Transformation();
  int rightVelocity, leftVelocity;
  angularVelocity = carDirection;
  // print something to debug
  Serial.println(carVelocity + (angularVelocity * carWidth) / 2);
  Serial.println(carVelocity - (angularVelocity * carWidth) / 2);
  Serial.println(rightVelocity);
  Serial.println(leftVelocity);
  // the formula to calculate wheels' velocity
  // carWidth is the length between two wheels, unit in meter
  // formula: 
  // 1. V_right = V_linear + (V_angular * wheel_distance) / 2
  // 2. V_left = V_linear - (V_angular * wheel_distance) / 2
  rightVelocity = mapfloat((carVelocity + (angularVelocity * carWidth) / 2), -0.062, 0.26, 0, 255); // calculate possible min and max value. map to correct pwm value
  leftVelocity = mapfloat((carVelocity - (angularVelocity * carWidth) / 2), -0.062, 0.26, 0, 255); // calculate possible min and max value. map to correct pwm value

  // set car stop
  if (carVelocity == 0)
  {
    analogWrite(speed_A, 0);
    digitalWrite(directon_A, HIGH);
    analogWrite(speed_B, 0);
    digitalWrite(directon_B, HIGH);
  }
  // if carDirection is positive and shouldn't backward
  // turn toward left front
  else if (carDirection > 0 && !isBackward)
  {
    analogWrite(speed_A, rightVelocity);
    digitalWrite(directon_A, HIGH);
    analogWrite(speed_B, leftVelocity);
    digitalWrite(directon_B, HIGH);
  }
  // if carDirection is negative and shouldn't backward
  // turn toward right front
  else if (carDirection <= 0 && !isBackward)
  {
    analogWrite(speed_A, rightVelocity);
    digitalWrite(directon_A, HIGH);
    analogWrite(speed_B, leftVelocity);
    digitalWrite(directon_B, HIGH);
  }
  // if carDirection is positive and should move backward
  // back the car toward left back
  else if (carDirection > 0 && isBackward)
  {
    analogWrite(speed_A, rightVelocity);
    digitalWrite(directon_A, LOW);
    analogWrite(speed_B, leftVelocity);
    digitalWrite(directon_B, LOW);
  }
  // if carDirection is negative and should move backward
  // back the car toward right back
  else if (carDirection <= 0 && isBackward)
  {
    analogWrite(speed_A, rightVelocity);
    digitalWrite(directon_A, LOW);
    analogWrite(speed_B, leftVelocity);
    digitalWrite(directon_B, LOW);
  }
}
void loop()
{
  RemoteXY_Handler();
  // if button press, kick the ball
  digitalWrite(n20_motor, 1 - RemoteXY.button_02);
  // control the servo
  targetAngle = map(RemoteXY.slider_01, 0, 100, 0, 180);
  if (servoAngle < targetAngle)
  {
    servoAngle = servoAngle + 5;
  }
  if (servoAngle > targetAngle)
  {
    servoAngle = servoAngle - 5;
  }
  myservo_00.write(servoAngle); // 旋轉到0度，就是一般所說的歸零
  myservo_01.write(servoAngle);

  // get joystick value one time for each loop
  joystick_x = RemoteXY.joystick_01_x;
  joystick_y = RemoteXY.joystick_01_y;
  // print out to check
  Serial.println(joystick_x);
  Serial.println(joystick_y);
  // control wheel
  motorControl();

  // use the RemoteXY structure for data transfer
  // do not call delay(), use instead RemoteXY_delay()
}