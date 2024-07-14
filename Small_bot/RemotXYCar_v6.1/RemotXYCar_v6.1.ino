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

const int speed_A = 4;     // right wheel
const int directon_A = 0;  // right wheel
const int speed_B = 5;     // left wheel
const int directon_B = 2;  // left wheel

bool isBackward = false;
float carDirection = 0;
float carVelocity = 0;
float angularVelocity = 0;
float carWidth = 0.205;

int motor = 15;
int joystickX = 0;
int joystickY = 0;
int RemoteXY_slider_01 = 0;
int yourservo = 0;
int angle = 0;
bool canWheelRun = false;
Servo servo_13;
Servo servo_12;

void ServoControl()
{
  yourservo = (map(RemoteXY_slider_01, 0, 100, 0, 180));
  if (angle < yourservo)
    angle = angle + 10;
  if (angle > yourservo)
    angle = angle - 10;
  servo_13.write(angle);
  servo_12.write((180 - angle));
}
inline void SetWheel(double rightVelocity, double leftVelocity, int a, int b)
{
  analogWrite(speed_A, rightVelocity);
  digitalWrite(directon_A, a);
  analogWrite(speed_B, leftVelocity);
  digitalWrite(directon_B, b);
}
// a map function, transform the wheel velocity velue to the range we want
inline float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}
// use current joystick value to calculate the car velocity and moving direction it should be.
void Transformation()
{
  // if joystick value small enough, set car velocity to 0
  if (abs(joystickX) <= 60 && abs(joystickY) <= 60)
  {
    carVelocity = 0;
    carDirection = 0;
    return;
  }
  // calculate the car velocity
  carVelocity = sqrt(joystickY * joystickY + joystickX * joystickX) / 1000;
  // determine if car have to move forward or backward. according to joystickX velue
  if (joystickY < 0)
    isBackward = true;
  else
    isBackward = false;
  // determine if car have to turn left or right. according to joystickY velue
  if (joystickX < 0)
    carDirection = atan(-joystickX / abs(joystickY));    // turn left. this value must be positive
  else if (joystickX >= 0)
    carDirection = -atan(joystickX / abs(joystickY));  // turn right. this value must be negative
}
// convert the car velocity and direction to the wheels' velocity
void MotorControl()
{
  // get the car velocity and moving direction it should be
  Transformation();
  angularVelocity = carDirection;

  int rightVelocity = mapfloat((carVelocity + (angularVelocity * carWidth) / 2), -0.062, 0.20, 0, 255); // calculate possible min and max value. map to correct pwm value
  int leftVelocity = mapfloat((carVelocity - (angularVelocity * carWidth) / 2), -0.062, 0.20, 0, 255); // calculate possible min and max value. map to correct pwm value

  if (carVelocity == 0)
    SetWheel(0, 0, 1, 1);
  else if (!isBackward)
    SetWheel(rightVelocity, leftVelocity, 1, 0);
  else if (carDirection > 0 && isBackward)
    SetWheel(rightVelocity, leftVelocity, 0, 1);
}
void setup()
{
  RemoteXY_Init();
  pinMode(speed_A, OUTPUT);
  pinMode(directon_A, OUTPUT);
  pinMode(speed_B, OUTPUT);
  pinMode(directon_B, OUTPUT);
  pinMode(motor, OUTPUT);
  servo_12.attach(12, 500, 2500);
  servo_13.attach(13, 500, 2500);
  servo_13.write(180);               // init servo
  servo_12.write(0);             // init servo
}
void loop()
{
  // update params
  RemoteXY_Handler();
  joystickX = RemoteXY.joystick_01_x;
  joystickY = RemoteXY.joystick_01_y;
  RemoteXY_slider_01 = RemoteXY.slider_01;

  // water motor
  digitalWrite(motor, RemoteXY.button_01);
  // servo control
  ServoControl();
  // wheels control
  MotorControl();
}
