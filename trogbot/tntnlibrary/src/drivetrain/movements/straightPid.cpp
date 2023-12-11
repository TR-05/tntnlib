#include <cmath>
#include <bits/stdc++.h>
#include "vex.h"
#include "../tntnlibrary/include/util.h"
#include "../tntnlibrary/include/drivetrain/pose.h"
#include "../tntnlibrary/include/drivetrain/movements/straightPid.h"
#include "../tntnlibrary/include/drivetrain/chassis/chassis.h"
#include "vex.h"

using namespace tntnlib;

/**
 * drive Pid parameters
 */

void straightPid::params(bool reversed, bool holdHeading, bool turnToPoint, float straightDistance, float lmaxSpeed, float amaxSpeed, float targetHeading, Pose targetPose)
{
    straightPid::startingVertical1Dist = sensors.vertical1->getDistance();
    straightPid::targetHeading = targetHeading;
    straightPid::straightDistance = straightDistance;
    straightPid::reversed = reversed;
    straightPid::lmaxSpeed = lmaxSpeed;
    straightPid::amaxSpeed = amaxSpeed;
    straightPid::holdHeading = holdHeading;
    straightPid::turnToPoint = turnToPoint;
    straightPid::targetPose = targetPose;
    straightPid::breakOutError = 0;
}

std::pair<float, float> straightPid::update(Pose pose)
{
    // if going in reverse, flip the heading of the pose
    if (reversed)
        pose.theta += 180;
    if (turnToPoint) {
        targetHeading = StandardFormRadToDeg(pose.angle(targetPose));
        //printf("targetHeading: %.2f\n tx: %.2f ty: %.2f", targetHeading, targetPose.x, targetPose.y);
    }

    // calculate error
    float angularError = angleError(targetHeading, pose.theta, false); // angular error
    float error = (straightDistance) - (sensors.vertical1->getDistance() - startingVertical1Dist);

    // get PID outputs
    float angularPower = angularPID.update(angularError, 0);
    float linearPower = linearPID.update(error, 0);
    if (holdHeading)
        angularPower = clamp(angularPower, -amaxSpeed, amaxSpeed);
    else {
        angularPower = 0;
    }
    linearPower = clamp(linearPower, -lmaxSpeed, lmaxSpeed);

    // prioritize turning over moving
    float overturn = fabs(angularPower) + fabs(linearPower) - lmaxSpeed;
    if (overturn > 0)
        linearPower -= (linearPower > 0 ? overturn : -overturn);

    // calculate motor powers
    float leftPower = linearPower + angularPower;
    float rightPower = linearPower - angularPower;

    // return motor output
    breakOutError = error;
    return {leftPower, rightPower};
}
