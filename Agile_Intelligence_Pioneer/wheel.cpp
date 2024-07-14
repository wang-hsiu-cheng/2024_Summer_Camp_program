#include "wheel.h"

using namespace std;

void pointToDist(const float xGoal, const float yGoal)
{
    goalDistanceX = xGoal - botPositionX;
    goalDistanceY = yGoal - botPositionY;
    return;
}
void initParam()
{
    xMoved = 0, yMoved = 0;
    remainX = lastRemainX = goalDistanceX;
    remainY = lastRemainY = goalDistanceY;
}
int moveTo()
{
    if (abs(remainX) > 0.001 && abs(lastRemainX) >= abs(remainX))
    {
        xMoved += realVelX * deltaTime;
        lastRemainX = remainX;
        remainX = goalDistanceX - xMoved;
        if (abs(xMoved) <= distance_p_control_0)
            xVelocityNow = velocity_p_control_0;
        else if (abs(xMoved) <= distance_p_control_1)
            xVelocityNow = velocity_p_control_1;
        else if (abs(xMoved) <= distance_p_control_2)
            xVelocityNow = velocity_p_control_2;
        else if (abs(remainX) <= distance_p_control_0)
            xVelocityNow = velocity_p_control_0;
        else if (abs(remainX) <= distance_p_control_1)
            xVelocityNow = velocity_p_control_1;
        else if (abs(remainX) <= distance_p_control_2)
            xVelocityNow = velocity_p_control_2;
        // It's too fast now, so I comment it
        // else if (remainDistance <= distance_p_control_3)
        //     xVelocityNow = velocity_p_control_3;
        else
            xVelocityNow = velocity_p_control_3;

        if (goalDistanceX < 0)
            VelX = -xVelocityNow;
        else
            VelX = xVelocityNow;
        return 0;
    }
    else
        VelX = 0;
    if (remainY > 0.001 && abs(lastRemainY) >= abs(remainY))
    {
        yMoved += realVelY * deltaTime;
        lastRemainY = remainY;
        remainY = goalDistanceX - yMoved;
        if (yMoved <= distance_p_control_0)
            yVelocityNow = velocity_p_control_0;
        else if (yMoved <= distance_p_control_1)
            yVelocityNow = velocity_p_control_1;
        else if (yMoved <= distance_p_control_2)
            yVelocityNow = velocity_p_control_2;
        else if (remainY <= distance_p_control_0)
            yVelocityNow = velocity_p_control_0;
        else if (remainY <= distance_p_control_1)
            yVelocityNow = velocity_p_control_1;
        else if (remainY <= distance_p_control_2)
            yVelocityNow = velocity_p_control_2;
        // It's too fast now, so I comment it
        // else if (remainY <= distance_p_control_3)
        //     yVelocityNow = velocity_p_control_3;
        else
            yVelocityNow = velocity_p_control_3;

        if (goalDistanceY < 0)
            VelY = -yVelocityNow;
        else
            VelY = yVelocityNow;
        return 0;
    }
    else
        VelY = 0;

    if (VelX == 0 && VelY == 0)
        return 1;
    else
        return 0;
}
// Not Used
int moveCurve(double radius, double angleRad)
{
    double xVelocityNow, zVelocityNow;
    double xVelocityBefore = 0, zVelocityBefore = 0;
    double angleNow = 0;
    double angleErr = angleRad;
    double angleConst;
    double xMoved = 0;
    double remainDistance = radius * abs(angleRad);
    float deltaTime = 0.001;

    while (abs(angleErr) > 0.02)
    {
        xVelocityBefore = realVelX;
        zVelocityBefore = realVelY;
        xMoved += xVelocityBefore * deltaTime;
        angleNow += zVelocityBefore * deltaTime;
        remainDistance = radius * abs(angleRad) - xMoved;
        angleErr = angleRad - angleNow;

        if (xMoved <= distance_p_control_0)
            xVelocityNow = velocity_p_control_0;
        else if (xMoved <= distance_p_control_1)
            xVelocityNow = velocity_p_control_1;
        else if (xMoved <= distance_p_control_2)
            xVelocityNow = velocity_p_control_2;
        else if (remainDistance <= distance_p_control_0)
            xVelocityNow = velocity_p_control_0;
        else if (remainDistance <= distance_p_control_1)
            xVelocityNow = velocity_p_control_1;
        else if (remainDistance <= distance_p_control_2)
            xVelocityNow = velocity_p_control_2;
        else
            xVelocityNow = velocity_p_control_3;
        angleConst = xVelocityNow / radius;
        if (angleRad < 0)
            angleConst = -abs(angleConst);

        VelX = xVelocityNow;
        AngVelW = angleConst;
    }
    return 1;
}
// Not Used
int stop()
{
    bool data_check, flag;
    float xVelocityNow, y_vel_now, z_vel_now;
    float xVelocityBefore = 0, y_vel_before = 0, z_vel_before = 0;
    data_check = false;
    int whole_speed = 0;

    while (!data_check)
    {
        float acceleration = -0.01;
        xVelocityBefore = realVelX;
        y_vel_before = realVelY;
        z_vel_before = realVelW;

        if (flag)
        {
            xVelocityNow = max((float)whole_speed, (xVelocityBefore + acceleration));
            y_vel_now = max((float)whole_speed, (y_vel_before + acceleration));
            z_vel_now = 0;
        }
        flag = true;
        VelX = xVelocityNow;
        VelY = y_vel_now;
        AngVelW = z_vel_now;
        if (xVelocityNow <= whole_speed && y_vel_now <= whole_speed)
            flag = false;
    }
    if (!flag)
        return 1;
    return 0;
}