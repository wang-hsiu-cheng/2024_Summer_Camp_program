//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// you can enable debug logging to Serial at 115200
//#define REMOTEXY__DEBUGLOG    

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__WIFI_POINT

#include <ESP8266WiFi.h>

// RemoteXY connection settings 
#define REMOTEXY_WIFI_SSID "RemoteXY_TEST"
#define REMOTEXY_WIFI_PASSWORD ""
#define REMOTEXY_SERVER_PORT 6377

#include <RemoteXY.h>

// RemoteXY GUI configuration  
#pragma pack(push, 1)  
uint8_t RemoteXY_CONF[] =   // 45 bytes
  { 255,4,0,0,0,38,0,17,0,0,0,0,1,200,84,1,1,3,0,4,
  18,53,52,17,192,2,26,1,25,16,19,19,0,2,31,0,5,128,18,55,
  55,39,2,26,31 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  int8_t slider_01; // from 0 to 100
  uint8_t button_01; // =1 if button pressed, else =0
  int8_t joystick_01_x; // from -100 to 100
  int8_t joystick_01_y; // from -100 to 100

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;   
#pragma pack(pop)
 
/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

#include <Servo.h>

int n20_motor = 15;
int RemoteXY_joystick_01_x = 0;
int RemoteXY_joystick_01_y = 0;
int RemoteXY_slider_01 = 0;
int yourservo = 0;
int angel = 0;
bool canWheelRun = false;
Servo servo_13;
Servo servo_12;

void ServoControl()
{
  yourservo = (map(RemoteXY_slider_01, 0, 100, 0, 180));
  if (angel < yourservo)
  {
    angel = angel + 10;
  }
  if (angel > yourservo)
  {
    angel = angel - 10;
  }
  servo_13.write(angel);
  servo_12.write((180 - angel));
}
bool MotorControl()
{
  if (abs(RemoteXY_joystick_01_x) <= 60 && abs(RemoteXY_joystick_01_y) <= 60)
  { 
    analogWrite(4, 0);
    digitalWrite(0, LOW);
    analogWrite(5, 0);
    digitalWrite(2, LOW);
    return false;
  }
  if (abs(RemoteXY_joystick_01_x) < abs(RemoteXY_joystick_01_y) && RemoteXY_joystick_01_y > 0) // forward
  {
    analogWrite(4, 150);   // right wheel, A+ A-
    digitalWrite(0, HIGH);  
    analogWrite(5, 150);   // left wheel, B+ B-
    digitalWrite(2, LOW);
  }
  else if (abs(RemoteXY_joystick_01_x) > abs(RemoteXY_joystick_01_y) && RemoteXY_joystick_01_x < 0) // left turn
  {
    analogWrite(4, 150);
    digitalWrite(0, HIGH);
    analogWrite(5, 150);
    digitalWrite(2, HIGH);
  }
  else if (abs(RemoteXY_joystick_01_x) < abs(RemoteXY_joystick_01_y) && RemoteXY_joystick_01_y < 0) // backward
  {
    analogWrite(4, 150);
    digitalWrite(0, LOW);
    analogWrite(5, 150);
    digitalWrite(2, HIGH);
  }
  else if (abs(RemoteXY_joystick_01_x) > abs(RemoteXY_joystick_01_y) && RemoteXY_joystick_01_x > 0) // right turn
  {
    analogWrite(4, 150);
    digitalWrite(0, LOW);
    analogWrite(5, 150);
    digitalWrite(2, LOW);
  }
  else
  {
    analogWrite(4, 0);
    digitalWrite(0, LOW);
    analogWrite(5, 0);
    digitalWrite(2, LOW);
  }
  return true;
}
void setup()
{
  RemoteXY_Init();
  pinMode(3, OUTPUT);
  pinMode(0, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(n20_motor, OUTPUT);
  servo_12.attach(12, 500, 2500);
  servo_13.attach(13, 500, 2500);
  servo_13.write(0);               // init servo
  servo_12.write(180);             // init servo
}
void loop()
{
  // update params
  RemoteXY_Handler();
  RemoteXY_joystick_01_x = RemoteXY.joystick_01_x;
  RemoteXY_joystick_01_y = RemoteXY.joystick_01_y;
  RemoteXY_slider_01 = RemoteXY.slider_01;

  // water motor
  digitalWrite(n20_motor, RemoteXY.button_01);
  // servo control
  ServoControl();
  // wheels control
  MotorControl();

  // TODO you loop code
  // use the RemoteXY structure for data transfer
  // do not call delay(), use instead RemoteXY_delay()
}
