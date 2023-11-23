#include <cmath>
#include <bits/stdc++.h>
#include "vex.h"
#include "../tntnlibrary/include/util.h"
#include "../tntnlibrary/include/pose.h"
#include "../tntnlibrary/include/movements/straightPid.h"
#include "vex.h"

using namespace tntnlib;

/**
 * Turn constructor
 *
 * Some members of the class need to be explicitly initialized
 * But, some members need to be configured further in the body
 *
 * Here we just store the arguments in member variables, and store the
 * initial competition state.
 */

void straightPid::params(bool reversed, float lmaxSpeed, float amaxSpeed)
{
    straightPid::reversed = reversed;
    straightPid::lmaxSpeed = lmaxSpeed;
    straightPid::amaxSpeed = amaxSpeed;
}

std::pair<float, float> straightPid::update(Pose pose)
{



    // if going in reverse, flip the heading of the pose
    if (reversed)
        pose.theta += 180;

    float targetHeadingRad = degToStandardFormRad(targetHeading);
    float poseError = pose.distance(targetPose);
    
    // calculate error
    float angularError = angleError(StandardFormRadToDeg(pose.angle(carrot)), pose.theta, false); // angular error
    float linearError = poseError * cos(degToRad(angularError));                                  // linear error

    if (state == 1)
    {
        angularError = angleError(targetPose.theta, pose.theta, false); // settling behavior
        poseError = poseError * cos(degToRad(angularError));
    }
    else
    {
        poseError = pose.distance(targetPose);
    }

    if (reversed)
        linearError = -linearError;

    // get PID outputs
    float angularPower = angularPID.update(angularError, 0);
    float linearPower = linearPID.update(linearError, 0);
    angularPower = clamp(angularPower, -amaxSpeed, amaxSpeed);
    linearPower = clamp(linearPower, -lmaxSpeed, lmaxSpeed);

    // calculate radius of turn
    float curvature = fabs(getCurvature(pose, carrot));
    if (curvature == 0)
        curvature = -1;
    float radius = 1 / curvature;

    // calculate the maximum speed at which the robot can turn
    // using the formula v = sqrt( u * r * g )
    if (radius != -1)
    {
        float maxTurnSpeed = sqrt(chasePower * radius * 9.8);
        // the new linear power is the minimum of the linear power and the max turn speed
        if (linearPower > maxTurnSpeed && state == 0)
            ;//linearPower = maxTurnSpeed;
        else if (linearPower < -maxTurnSpeed && state == 0)
            ;//linearPower = -maxTurnSpeed;
    }

    // prioritize turning over moving
    float overturn = fabs(angularPower) + fabs(linearPower) - lmaxSpeed;
    if (overturn > 0)
        linearPower -= (linearPower > 0 ? overturn : -overturn);

    // calculate motor powers
    float leftPower = linearPower + angularPower;
    float rightPower = linearPower - angularPower;

    // return motor output
    breakOutError = poseError;
    return {leftPower, rightPower};
}
