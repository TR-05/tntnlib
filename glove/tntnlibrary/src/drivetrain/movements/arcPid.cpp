#include "../tntnlibrary/include/drivetrain/movements/arcPid.h"

#include <bits/stdc++.h>

#include <cmath>

#include "../tntnlibrary/include/drivetrain/chassis/chassis.h"
#include "../tntnlibrary/include/drivetrain/pose.h"
#include "../tntnlibrary/include/util.h"
#include "vex.h"

using namespace tntnlib;

float arcPid::arclength = 0, arcPid::startingVertical1Dist = 0, arcPid::startTimeMs = 0, arcPid::startTheta = 0, arcPid::endTheta = 0;
float arcPid::radius = 0, arcPid::lmaxSpeed = 0, arcPid::breakOutError = 0;
int arcPid::direction = 0;
bool arcPid::reversed = false;
Profile arcPid::linearProfile(0, 0, 0, 0, 0);

void arcPid::params(float arclength, float radius, int direction, float lmaxSpeed, float breakDist)
{
    arcPid::startingVertical1Dist = chassis.sensors.vertical1->getDistance();
    arcPid::arclength = arclength;
    arcPid::reversed = false;
    arcPid::lmaxSpeed = lmaxSpeed;
    arcPid::radius = radius;
    arcPid::breakOutError = 0;
    arcPid::direction = direction;
    // arcPid::linearProfile = Profile(arclength, vMax, vi, vf, a);
    arcPid::startTimeMs = tntnlib::time(true);
    arcPid::startTheta = chassis.getPose().theta;
    arcPid::endTheta = arcPid::startTheta + radToDeg((arcPid::arclength / arcPid::radius) * arcPid::direction);
    int count = 0;
    for (int i = 0; i <= arcPid::linearProfile.t * 1000.0 / 10.0; i++)
    {
        count++;
        if (count % 5 == 0)
        {
            printf("\n");
        }
        printf("(%.2f, %.2f),", i * 10 / 1000.0, arcPid::linearProfile.V[i]);
        delay(3);
    }
    printf("\n\n\n\n");
}

std::pair<float, float> arcPid::update(Pose pose)
{
    float t = (tntnlib::time(true) - arcPid::startTimeMs);
    int roundedT = static_cast<int>(t) / 10;  // wizard stuff from copilot :D
    float v = arcPid::linearProfile.V[roundedT];
    if (t / 1000.0 > arcPid::linearProfile.t)
    {
        v = 0;
    }

    float error = (tntnlib::arcPid::arclength) - (chassis.sensors.vertical1->getDistance() - startingVertical1Dist);
    // get PID outputs
    float linearPower = tntnlib::linearPID.update(error, 0);  //* pow((arclength - error) / arclength, 3);
    linearPower = clamp(linearPower, -arcPid::lmaxSpeed, arcPid::lmaxSpeed);
    v = chassis.drivetrain.leftMotors->voltsToTipVelocity(linearPower);

    // differntial drive kinematics used below can be found here:
    // https://cs.columbia.edu/~allen/F15/NOTES/icckinematics.pdf
    float L = chassis.drivetrain.trackWidth;
    float vInner = v - ((v * L) / (2 * arcPid::radius));
    float vOuter = v + ((v * L) / (2 * arcPid::radius));

    float leftPower = 0;
    float rightPower = 0;
    if (arcPid::direction == -1)
    {
        leftPower = vInner;
        rightPower = vOuter;
    }
    else if (arcPid::direction == 1)
    {
        leftPower = vOuter;
        rightPower = vInner;
    }
    else if (arcPid::direction == 0)
    {
        leftPower = v;
        rightPower = v;
    }

    leftPower = chassis.drivetrain.leftMotors->getPower(chassis.drivetrain.leftMotors->tipVelocityToRPM(leftPower));
    rightPower = chassis.drivetrain.rightMotors->getPower(chassis.drivetrain.rightMotors->tipVelocityToRPM(rightPower));

    // calculate error
    float angularError = arcPid::endTheta - pose.theta;
    angularError = fmod(angularError, 360);
    if (angularError > 180)
        angularError -= 360;
    else if (angularError < -180)
        angularError += 360;

    // calculate the speed
    // converts error to degrees to make PID tuning easier
    float angularPower = angularPID.update(angularError, 0);
    // cap the speed
    angularPower = clamp(angularPower, -12, 12);

    if (fabs(error) < 4)
    {
        leftPower = linearPower + angularPower;
        rightPower = linearPower - angularPower;
    }

    // return motor output
    tntnlib::arcPid::breakOutError = error;
    // printf("leftPower: %.2f, rightPower: %.2f, error: %.2f\n", leftPower, rightPower, error);
    static int count = 0;
    count++;

    printf("(%.2f, %.2f),", time() - arcPid::startTimeMs / 1000.0, chassis.drivetrain.leftMotors->getTipVelocity());
    printf("(%.2f, %.2f),", time() - arcPid::startTimeMs / 1000.0, arcPid::arclength - error);

    std::cout << std::flush;
    if (count % 3 == 0)
    {
        printf("\n");
    }

    return {leftPower, rightPower};
}
