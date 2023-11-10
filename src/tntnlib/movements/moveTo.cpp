#include <cmath>
#include <bits/stdc++.h>
#include "vex.h"
#include "tntnlib/util.h"
#include "tntnlib/pose.h"
#include "tntnlib/movements/moveTo.h"
#include "vex.h"

float clamp(float input, float min, float max)
{
    float output;
    if (input > max)
        output = max;
    else if (input < min)
        output = min;
    else
    {
        output = input;
    }
    return output;
}

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

void MoveTo::params(float target, bool reversed, float maxSpeed, bool swingOnLeft, bool swingOnRight)
{
    moveSettings.targetHeading = target;
    moveSettings.reversed = reversed;
    moveSettings.maxSpeed = maxSpeed;
    moveSettings.useHeading = true;
    moveSettings.swingOnLeft = swingOnLeft;
    moveSettings.swingOnRight = swingOnRight;
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

void MoveTo::params(Pose target, bool reversed, float maxSpeed, bool swingOnLeft, bool swingOnRight)
{
    moveSettings.targetPose = target;
    moveSettings.reversed = reversed;
    moveSettings.maxSpeed = maxSpeed;
    moveSettings.useHeading = false;
    moveSettings.swingOnLeft = swingOnLeft;
    moveSettings.swingOnRight = swingOnRight;
}

/**
 * The turning algorithm uses field-relative position of the robot to face a target heading
 * or face a target point.
 *
 * This algorithm is simple. When the robot needs to face a target heading, it simply aligns
 * the robot's heading with the target heading. When the robot is turning to face a point,
 * the algorithm will align the robot's heading with the target point. This is better for
 * repeatability, but is not always necessary.
 *
 * This algorithm only uses 1 PID to turn the chassis.
 */
std::pair<float, float> tntnlib::MoveTo::update(Pose pose)
{
    float t;
    if (moveSettings.useHeading)
        t = moveSettings.targetHeading;
    else
    {
        t = StandardFormRadToDeg(pose.angle(moveSettings.targetPose));
    }

    t = fmod(t, 360);

    // reverse heading if doing movement in reverse
    if (moveSettings.reversed)
    {
        pose.theta = fmod(pose.theta - 180, 360);
    }

    // update completion vars
    if (moveSettings.dist == 0)
    { // if dist is 0, this is the first time update() has been called
        moveSettings.dist = 0.0001;
        moveSettings.startPose = pose;
    }

    // calculate error
    float error = angleError(t, pose.theta, false);
    // calculate distance travelled
    moveSettings.dist = fabs(error);

    // calculate the speed
    // converts error to degrees to make PID tuning easier
    float output = angularPID.update(error, 0);
    // cap the speed
    output = clamp(output, -moveSettings.maxSpeed, moveSettings.maxSpeed);

    // return output
    if (moveSettings.swingOnLeft)
        return {0, -output};
    if (moveSettings.swingOnRight)
        return {output, 0};
    return {output, -output};
}
