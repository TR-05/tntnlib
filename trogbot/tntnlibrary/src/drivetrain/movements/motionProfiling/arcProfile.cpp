#include <cmath>
#include <bits/stdc++.h>
#include "vex.h"
#include "../tntnlibrary/include/util.h"
#include "../tntnlibrary/include/drivetrain/pose.h"
#include "../tntnlibrary/include/drivetrain/movements/motionProfiling/arcProfile.h"
#include "../tntnlibrary/include/drivetrain/chassis/chassis.h"
#include "vex.h"

using namespace tntnlib;

float arcProfile::arclength = 0, arcProfile::startingVertical1Dist = 0, arcProfile::startTimeMs = 0;
float arcProfile::radius = 0, arcProfile::vMax = 0, arcProfile::breakOutError = 0;
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
}

std::pair<float, float> arcProfile::update(Pose pose)
{
    float t = (tntnlib::time(true) - arcProfile::startTimeMs);
    int roundedT = static_cast<int>(t) / 10 * 10; // wizard stuff from copilot :D
    float v = arcProfile::linearProfile.V[roundedT];
    //differntial drive kinematics used below can be found here:
    //https://cs.columbia.edu/~allen/F15/NOTES/icckinematics.pdf
    float L = chassis.drivetrain.trackWidth;
    float vInner = v*(arcProfile::radius - (L/2));
    float vOuter = v*(arcProfile::radius + (L/2));

    if (arcProfile::direction == -1)
    {

    }
    else 
    {

    }
    // if going in reverse, flip the heading of the pose
    if (arcProfile::reversed)
        pose.theta += 180;

    float error = (tntnlib::arcProfile::arclength) - (chassis.sensors.vertical1->getDistance() - startingVertical1Dist);

    // get PID outputs
    float linearPower = tntnlib::linearPID.update(error, 0);
    float angularPower = tntnlib::angularPID.update(0, 0);
    linearPower = clamp(linearPower, -vMax, vMax);

    // calculate motor powers
    float leftPower = linearPower + angularPower;
    float rightPower = linearPower - angularPower;

    // return motor output
    tntnlib::arcProfile::breakOutError = error;
    return {leftPower, rightPower};
}
