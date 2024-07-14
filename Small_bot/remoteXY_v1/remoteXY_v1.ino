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

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// you can enable debug logging to Serial at 115200
// #define REMOTEXY__DEBUGLOG

// RemoteXY select connection mode and include library
#define REMOTEXY_MODE__ESP8266WIFI_LIB_POINT

#include <ESP8266WiFi.h>

// RemoteXY connection settings
#define REMOTEXY_WIFI_SSID "RemoteXY"
#define REMOTEXY_WIFI_PASSWORD "12345678"
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
  uint8_t button_02;    // =1 if button pressed, else =0
  int8_t slider_01;     // from 0 to 100

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
const int servo_0 = D7;     // right gate
const int servo_1 = D6;     // left gate

bool isBackward = false;
int targetAngle;
int servoAngle = 0;
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

void Transformation()
{
  if (RemoteXY.joystick_01_x == 0 && RemoteXY.joystick_01_y == 0)
  {
    carVelocity = 0;
    carDirection = 0;
    return;
  }
  carVelocity = sqrt(RemoteXY.joystick_01_y * RemoteXY.joystick_01_y + RemoteXY.joystick_01_x * RemoteXY.joystick_01_x) / 1000;
  if (RemoteXY.joystick_01_x < 0)
    isBackward = true;
  if (RemoteXY.joystick_01_y > 0)
  {
    carDirection = atan(RemoteXY.joystick_01_y / abs(RemoteXY.joystick_01_x));
    // if (RemoteXY.joystick_01_x < 0)
    //   carDirection = PI() - carDirection;
  }
  else if (RemoteXY.joystick_01_y <= 0)
  {
    carDirection = -atan(-RemoteXY.joystick_01_y / abs(RemoteXY.joystick_01_x));
    // if (RemoteXY.joystick_01_x < 0)
    //   carDirection = carDirection - PI();
  }
}
void motorControl()
{
  int rightVelocity, leftVelocity;
  angularVelocity = carDirection;
  rightVelocity = (carVelocity + (angularVelocity * carWidth) / 2) * 800;
  leftVelocity = (carVelocity - (angularVelocity * carWidth) / 2) * 800;

  if (carDirection > 0 && !isBackward)
  {
    digitalWrite(speed_A, rightVelocity);
    digitalWrite(directon_A, HIGH);
    digitalWrite(speed_B, leftVelocity);
    digitalWrite(directon_B, HIGH);
  }
  else if (carDirection <= 0 && !isBackward)
  {
    digitalWrite(speed_A, rightVelocity);
    digitalWrite(directon_A, HIGH);
    digitalWrite(speed_B, leftVelocity);
    digitalWrite(directon_B, HIGH);
  }
  else if (carDirection > 0 && isBackward)
  {
    digitalWrite(speed_A, rightVelocity);
    digitalWrite(directon_A, LOW);
    digitalWrite(speed_B, leftVelocity);
    digitalWrite(directon_B, LOW);
  }
  else if (carDirection <= 0 && isBackward)
  {
    digitalWrite(speed_A, rightVelocity);
    digitalWrite(directon_A, LOW);
    digitalWrite(speed_B, leftVelocity);
    digitalWrite(directon_B, LOW);
  }

}
void loop()
{
  RemoteXY_Handler();
  digitalWrite(speed_A, HIGH);
  digitalWrite(directon_A, HIGH);
  digitalWrite(n20_motor, 1 - RemoteXY.button_02);
  Serial.println("test");
  Serial.println(1 - RemoteXY.button_02);

  targetAngle = map(RemoteXY.slider_01, 0, 100, 0, 180);
  Serial.println(targetAngle);

  digitalWrite(n20_motor, 1 - RemoteXY.button_02);
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

  // TODO you loop code
  // use the RemoteXY structure for data transfer
  // do not call delay(), use instead RemoteXY_delay()
}