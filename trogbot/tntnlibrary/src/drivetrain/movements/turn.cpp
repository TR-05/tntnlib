#include <cmath>
#include <bits/stdc++.h>
#include "vex.h"
#include "../tntnlibrary/include/util.h"
#include "../tntnlibrary/include/drivetrain/pose.h"
#include "../tntnlibrary/include/drivetrain/movements/turn.h"
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

void Turn::params(float target, bool reversed, float maxSpeed, bool swingOnLeft, bool swingOnRight, bool boundto360)
{
    turnSettings.targetHeading = target;
    turnSettings.reversed = reversed;
    turnSettings.maxSpeed = maxSpeed;
    turnSettings.useHeading = true;
    turnSettings.swingOnLeft = swingOnLeft;
    turnSettings.swingOnRight = swingOnRight;
    turnSettings.boundto360 = boundto360;
}

/**
 * Turn constructor
 *
 * Some members of the class need to be explicitly initialized
 * But, some members need to be configured further in the body
 *
 * Here we just store the arguments in member variables, and store the
 * initial competition state.
 */

void Turn::params(Pose target, bool reversed, float maxSpeed, bool swingOnLeft, bool swingOnRight, bool boundto360)
{
    turnSettings.targetPose = target;
    turnSettings.reversed = reversed;
    turnSettings.maxSpeed = maxSpeed;
    turnSettings.useHeading = false;
    turnSettings.swingOnLeft = swingOnLeft;
    turnSettings.swingOnRight = swingOnRight;
    turnSettings.boundto360 = boundto360;
}

std::pair<float, float> tntnlib::Turn::update(Pose pose)
{
    float t;
    if (turnSettings.useHeading)
        t = turnSettings.targetHeading;
    else
    {
        t = StandardFormRadToDeg(pose.angle(turnSettings.targetPose));
    }

    // reverse heading if doing movement in reverse
    if (turnSettings.reversed)
    {
        pose.theta = pose.theta - 180;
    }

    // update completion vars
    if (turnSettings.dist == 0)
    { // if dist is 0, this is the first time update() has been called
        turnSettings.dist = 0.0001;
        turnSettings.startPose = pose;
    }

    // calculate error
    float error = t - pose.theta;
    if (turnSettings.boundto360)
    {
        error = fmod(error, 360);
        if (error > 180)
            error -= 360;
        else if (error < -180)
            error += 360;
    }

    // calculate distance travelled
    turnSettings.dist = fabs(error);

    // calculate the speed
    // converts error to degrees to make PID tuning easier
    float output = angularPID.update(error, 0);
    // cap the speed
    output = clamp(output, -turnSettings.maxSpeed, turnSettings.maxSpeed);
    breakOutError = error;
    // return output
    if (turnSettings.swingOnLeft)
        return {0, -output};
    if (turnSettings.swingOnRight)
        return {output, 0};
    return {output, -output};
}
