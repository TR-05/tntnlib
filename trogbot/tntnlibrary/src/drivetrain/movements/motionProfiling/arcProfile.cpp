#include "../tntnlibrary/include/drivetrain/movements/motionProfiling/arcProfile.h"

#include <bits/stdc++.h>

#include <cmath>

#include "../tntnlibrary/include/drivetrain/chassis/chassis.h"
#include "../tntnlibrary/include/drivetrain/pose.h"
#include "../tntnlibrary/include/util.h"
#include "vex.h"

using namespace tntnlib;

float arcProfile::arclength = 0, arcProfile::startingVertical1Dist = 0, arcProfile::startTimeMs = 0;
float arcProfile::radius = 0, arcProfile::vMax = 0, arcProfile::breakOutError = 0;
int arcProfile::direction = 0;
bool arcProfile::reversed = false;
Profile arcProfile::linearProfile(0, 0, 0, 0, 0);

void arcProfile::params(int direction, bool reversed, float arclength, float vMax, float vi, float vf, float a, float radius)
{
    arcProfile::startingVertical1Dist = chassis.sensors.vertical1->getDistance();
    arcProfile::arclength = arclength;
    arcProfile::reversed = reversed;
    arcProfile::vMax = vMax;
    arcProfile::radius = radius;
    arcProfile::breakOutError = 0;
    arcProfile::direction = direction;
    arcProfile::linearProfile = Profile(arclength, vMax, vi, vf, a);
    arcProfile::startTimeMs = tntnlib::time(true);
    int count = 0;
    for (int i = 0; i <= arcProfile::linearProfile.t*1000.0/10.0; i++)
    {
        count++;
        if (count % 5 == 0)
        {
            printf("\n");
        }
        printf("(%.2f, %.2f),", i*10/1000.0, arcProfile::linearProfile.V[i]);
        delay(3);
    }
            printf("\n\n\n\n");
}

std::pair<float, float> arcProfile::update(Pose pose)
{
    float t = (tntnlib::time(true) - arcProfile::startTimeMs);
    int roundedT = static_cast<int>(t) / 10;  // wizard stuff from copilot :D
    float v = arcProfile::linearProfile.V[roundedT];
    if (t / 1000.0 > arcProfile::linearProfile.t)
    {
        return {0, 0};
    }




    // differntial drive kinematics used below can be found here:
    // https://cs.columbia.edu/~allen/F15/NOTES/icckinematics.pdf
    float L = chassis.drivetrain.trackWidth;
    float vInner = v - ((v * L) / (2 * arcProfile::radius));
    float vOuter = v + ((v * L) / (2 * arcProfile::radius));

    float leftPower = 0;
    float rightPower = 0;
    if (arcProfile::direction == -1)
    {
        leftPower = vInner;
        rightPower = vOuter;
    }
    else if (arcProfile::direction == 1)
    {
        leftPower = vOuter;
        rightPower = vInner;
    }
    else if (arcProfile::direction == 0)
    {
        leftPower = v;
        rightPower = v;
    }


    float angularPower = tntnlib::angularPID.update(0, 0);

    leftPower = chassis.drivetrain.leftMotors->getPower(chassis.drivetrain.leftMotors->tipVelocityToRPM(leftPower));
    rightPower = chassis.drivetrain.rightMotors->getPower(chassis.drivetrain.rightMotors->tipVelocityToRPM(rightPower));

    float error = (tntnlib::arcProfile::arclength) - (chassis.sensors.vertical1->getDistance() - startingVertical1Dist);
    // get PID outputs
    float linearPower = tntnlib::linearPID.update(error, 0); //* pow((arclength - error) / arclength, 3);
    linearPower = clamp(linearPower, -2, 2);
    if (fabs(error) < arcProfile::arclength/2)
    {
        if (fabs(chassis.drivetrain.leftMotors->getPower(chassis.drivetrain.leftMotors->tipVelocityToRPM(v)) < 2) )
        {
            leftPower = linearPower;
            rightPower = linearPower;
        }
    }

    // return motor output
    tntnlib::arcProfile::breakOutError = error;
    // printf("leftPower: %.2f, rightPower: %.2f, error: %.2f\n", leftPower, rightPower, error);
    static int count = 0;
    count++;

    printf("(%.2f, %.2f),", time() - arcProfile::startTimeMs/1000.0, chassis.drivetrain.leftMotors->getTipVelocity());
    printf("(%.2f, %.2f),", time() - arcProfile::startTimeMs/1000.0, arcProfile::arclength - error);

    std::cout << std::flush;
    if (count % 3 == 0)
    {
        printf("\n");
    }

    return {leftPower, rightPower};
}
