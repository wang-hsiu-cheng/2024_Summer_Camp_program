#define REMOTEXY_MODE__WIFI_POINT

#include <ESP8266WiFi.h>

// RemoteXY connection settings 
#define REMOTEXY_WIFI_SSID "RemoteXY_12_1"
#define REMOTEXY_WIFI_PASSWORD ""
#define REMOTEXY_SERVER_PORT 6377
#define MOTOR_CD 60

#include <RemoteXY.h>

int  n20_motor = 15;
int  RemoteXY_joystick_01_x = 0;
int  RemoteXY_joystick_01_y = 0;
int  RemoteXY_slider_01 = 0;
int  RemoteXY_button_01 = 0;
int  yourservo=0;
int  angel=0;
int  waterWheel = MOTOR_CD;
#include <Servo.h>
Servo servo_13;

Servo servo_12;
// RemoteXY GUI configuration  
#pragma pack(push, 1)  
uint8_t RemoteXY_CONF[] =   // 45 bytes
  { 255,4,0,0,0,38,0,17,0,0,0,0,1,106,200,1,1,3,0,1,
  37,79,24,24,0,2,31,0,4,46,16,14,36,0,2,26,5,21,121,60,
  60,32,2,26,31 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t button_01; // =1 if button pressed, else =0
  int8_t slider_01; // from 0 to 100
  int8_t joystick_01_x; // from -100 to 100
  int8_t joystick_01_y; // from -100 to 100

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;   
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////



void setup()
{
  RemoteXY_Init ();
  pinMode(3, OUTPUT);
  pinMode(0, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(n20_motor, OUTPUT);
  servo_12.attach(12,500,2500);
  servo_13.attach(13,500,2500);
  // TODO you setup code
  servo_13.write(0); 
  servo_12.write(180); 
}
void loop()
{
  RemoteXY_Handler ();
  RemoteXY_joystick_01_x = RemoteXY.joystick_01_x;
  RemoteXY_joystick_01_y = RemoteXY.joystick_01_y;
  RemoteXY_slider_01 = RemoteXY.slider_01;
  RemoteXY_button_01 = RemoteXY.button_01;
//  if (waterWheel > 0)
//  {
//    digitalWrite(n20_motor, RemoteXY_button_01);
//    waterWheel--;
//  }
//  else if (waterWheel > -MOTOR_CD)
//  {
//    digitalWrite(n20_motor, 0);
//    waterWheel--;
//  }
//  else
//    waterWheel = MOTOR_CD;


  if (waterWheel > 0)
  {
    digitalWrite(n20_motor, RemoteXY_button_01);
    waterWheel--;
  }
  else if (waterWheel > -MOTOR_CD)
  {
    digitalWrite(n20_motor, 0);
    waterWheel--;
  }
  else
  {
    waterWheel = MOTOR_CD;
  }
  
  yourservo = (map(RemoteXY_slider_01, 0, 100, 0, 180));
  if (angel < yourservo) {
    angel = angel + 10;
  }
  if (angel > yourservo) {
    angel = angel - 10;
  }
  
  servo_13.write(angel);  
  
  servo_12.write((180 - angel));
  if (abs(RemoteXY_joystick_01_x) > 60 || abs(RemoteXY_joystick_01_y) > 60) {
    if (abs(RemoteXY_joystick_01_x) < abs(RemoteXY_joystick_01_y) && RemoteXY_joystick_01_y > 0) {
      analogWrite(4, 100);
      digitalWrite(0, HIGH);
      analogWrite(5, 100);
      digitalWrite(2, HIGH);
    } else if (abs(RemoteXY_joystick_01_x) > abs(RemoteXY_joystick_01_y) && RemoteXY_joystick_01_x < 0) {
      analogWrite(4, 200);
      digitalWrite(0, HIGH);
      analogWrite(5, 200);
      digitalWrite(2, LOW);
    } else if (abs(RemoteXY_joystick_01_x) < abs(RemoteXY_joystick_01_y) && RemoteXY_joystick_01_y < 0) {
      analogWrite(4, 100);
      digitalWrite(0, LOW);
      analogWrite(5, 100);
      digitalWrite(2, LOW);
    } else if (abs(RemoteXY_joystick_01_x) > abs(RemoteXY_joystick_01_y) && RemoteXY_joystick_01_x > 0) {
      analogWrite(4, 200);
      digitalWrite(0, LOW);
      analogWrite(5, 200);
      digitalWrite(2, HIGH);
    } else {
      analogWrite(4, 0);
      digitalWrite(0, LOW);
      analogWrite(5, 0);
      digitalWrite(2, LOW);
    }
  } else {
    analogWrite(4, 0);
    digitalWrite(0, HIGH);
    analogWrite(5, 0);
    digitalWrite(2, LOW);
  }

  // TODO you loop code
  // use the RemoteXY structure for data transfer
  // do not call delay(), use instead RemoteXY_delay()


}
