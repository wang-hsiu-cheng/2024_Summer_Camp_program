#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <array>
#include <cstdlib>
#include <ctime>
#include "wheel.cpp"

const int pointNumbers = 6;
const float points[pointNumbers][3] = {{0, 1, 1},
                                       {0, 1, 4},
                                       {0, 1, 1},
                                       {0, 4, 1},
                                       {0, 4, 4},
                                       {0, 0, 0}};
int currentPoint = 0;
int *pointPath = new int[pointNumbers];
int arrived = 0;

void loop();
void pointsPlanner();

// Only for STM
// HAL_TIM_Base_Start_IT(&htim3);
int main()
{
    // Init points order & distance for first movement
    pointsPlanner();
    goalDistanceX = points[pointPath[currentPoint]][1];
    goalDistanceY = points[pointPath[currentPoint]][2];
    while (1)
    {
        loop();
    }
    return 0;
}
// Only for STM
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM1)
    {
        if (arrived == 1)
        {
            pointToDist(points[pointPath[++currentPoint]][1], points[pointPath[++currentPoint]][2]);
            initParam();
        }
        arrived = moveTo();
        t_ms++;
    }
}
void initPath(float distanceArray[pointNumbers][pointNumbers], int pointPath[pointNumbers], int newPointPath[pointNumbers])
{
    for (int i = 0; i < pointNumbers; i++)
        for (int j = 0; j < pointNumbers; j++)
            distanceArray[i][j] = sqrt(pow((points[i][1] - points[j][1]), 2) + pow((points[i][2] - points[j][2]), 2));
    for (int i = 0; i < pointNumbers; i++)
        newPointPath[i] = pointPath[i] = i;
    return;
}
void inverseTwoPoint(int newPointPath[pointNumbers])
{
    int num1, num2;

    srand(time(0));
    num1 = rand() % (pointNumbers - 1) + 1;
    // Generate the second random number until it is different from the first
    do
    {
        num2 = rand() % (pointNumbers - 1) + 1;
    } while (num1 == num2);

    swap(newPointPath[num1], newPointPath[num2]);
    return;
}
float calDistance(float distanceArray[pointNumbers][pointNumbers], int newPointPath[pointNumbers])
{
    int sum = 0;
    for (int i = 0; i < pointNumbers - 1; i++)
        sum += distanceArray[newPointPath[i]][newPointPath[i + 1]];
    return sum;
}
void pointsPlanner()
{
    float distanceArray[pointNumbers][pointNumbers];
    int *newPointPath = new int[pointNumbers];
    int runs = 50;
    float r;
    float delta_f;
    float temperature = 1;
    float tempMin = 0.01;
    float R_t = 0.95;
    float weight = 0;

    initPath(distanceArray, pointPath, newPointPath);

    // Debug Print Out
    // for (int i = 0; i < pointNumbers; i++)
    // {
    //     for (int j = 0; j < pointNumbers; j++)
    //         printf("%f ", distanceArray[i][j]);
    //     printf("\n");
    // }
    // for (int i = 0; i < pointNumbers; i++)
    //     printf("(%.3f, %.3f)", points[pointPath[i]][1], points[pointPath[i]][2]);
    // printf("\n");

    while (1)
    {
        if (temperature <= tempMin)
            break;
        for (int i = 0; i < runs; i++)
        {
            inverseTwoPoint(newPointPath);
            // Debug Print Out
            // for (int i = 0; i < pointNumbers; i++)
            //     printf("(%.3f, %.3f)", points[pointPath[i]][1], points[pointPath[i]][2]);
            // printf("\n");
            float newWeight = calDistance(distanceArray, newPointPath);
            delta_f = weight - newWeight;
            if (delta_f <= 0)
            {
                pointPath = newPointPath;
                weight = newWeight;
            }
            else
            {
                srand(time(0));
                float probability = exp(-delta_f / temperature);
                r = (rand() % 1000) / 1000;
                if (r < probability)
                {
                    pointPath = newPointPath;
                    weight = newWeight;
                }
                else
                {
                    pointPath = pointPath;
                    weight = weight;
                }
            }
        }
        temperature *= R_t;
    }

    // Debug Print Out
    // for (int i = 0; i < pointNumbers; i++)
    //     printf("(%.3f, %.3f)", points[pointPath[i]][1], points[pointPath[i]][2]);
    // printf("\n");
    return;
}
void loop()
{
    return;
}