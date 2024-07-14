#ifndef _WHEEL_H_
#define _WHEEL_H_

#include <math.h>

void pointToDist(const float xGoal, const float yGoal);
void initParam();
int stop();
int moveTo();
int moveCurve(double radius, double angleRad);

float deltaTime = 0.001;
/* global param */
float realVelX = 0;
float realVelY = 0;
float realVelW = 0;
float botPositionX = 0;
float botPositionY = 0;
float goalDistanceX;
float goalDistanceY;
float VelX, VelY, AngVelW;
/* function param*/
float xVelocityNow, yVelocityNow, zVelocityNow;
float xMoved = 0, yMoved = 0;
float remainX = goalDistanceX;
float remainY = goalDistanceY;
float lastRemainX = goalDistanceX;
float lastRemainY = goalDistanceY;
/* p control param */
const float rad_p_control_0 = 0.1745329252;
const float rad_p_control_1 = 0.5235988;
const float rad_p_control_2 = 1.0471976;
const float rad_p_control_3 = 1.57079632679;
const float omega_p_control_0 = 0.087266462599716;
const float omega_p_control_1 = 0.174532925199433;
const float omega_p_control_2 = 0.349065850398866;
const float omega_p_control_3 = 0.523598775598299;
const float distance_p_control_0 = 0.05;
const float distance_p_control_1 = 0.1;
const float distance_p_control_2 = 0.25;
const float distance_p_control_3 = 0.5;
const float velocity_p_control_0 = 0.03;
const float velocity_p_control_1 = 0.08;
const float velocity_p_control_2 = 0.14;
const float velocity_p_control_3 = 0.13;
const float velocity_p_control_4 = 0.26;

#endif
