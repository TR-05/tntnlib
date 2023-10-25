#include <cmath>
#include <bits/stdc++.h>
#include "vex.h"
#include "tntnlib/util.h"
#include "tntnlib/pose.h"
#include "tntnlib/movements/turn.h"
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


void Turn::params(FAPID TurnPID, float target, bool reversed, float maxSpeed)
{
    turnPID = TurnPID;
    targetHeading = target;
    Turn::reversed = reversed;
    Turn::maxSpeed = maxSpeed;
}

/*
tntnlib::Turn::Turn(FAPID angularPID, float target, int maxSpeed)
    : angularPID(angularPID),
      targetHeading(target),
      maxSpeed(maxSpeed) {
}
*/
/**
 * Turn constructor
 *
 * Some members of the class need to be explicitly initialized
 * But, some members need to be configured further in the body
 *
 * Here we just store the arguments in member variables, and store the
 * initial competition state.
 */


void Turn::params(FAPID TurnPID, Pose target, bool reversed, float maxSpeed)
{
    turnPID = TurnPID;
    targetPose = target;
    Turn::reversed = reversed;
    Turn::maxSpeed = maxSpeed;
}

/*

tntnlib::Turn::Turn(FAPID angularPID, Pose target, bool reversed, int maxSpeed)
    : angularPID(angularPID),
      targetPose(target),
      reversed(reversed),
      maxSpeed(maxSpeed) {
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
std::pair<float, float> tntnlib::Turn::update(Pose pose)
{
    float t = targetHeading;

    // set state to 1 if the pid has settled
    if (turnPID.settled())
        ;//state = 1;
    // exit if movement is in state 1 (settled)
    if (state == 1)
        return {13, 0};

    // reverse heading if doing movement in reverse
    if (reversed)
    {
        pose.theta = fmod(pose.theta - M_PI, 2 * M_PI);
        t = fmod(degToRad(t) - M_PI, 2 * M_PI);
    }
    else
    {
        t = fmod(degToRad(t), 2 * M_PI);
    }

    // update completion vars
    if (dist == 0)
    { // if dist is 0, this is the first time update() has been called
        dist = 0.0001;
        startPose = pose;
    }

    // calculate error
    float error = angleError(t, pose.theta);
   // printf("E:%.2f  TH:%.2f  BH:%.2f", radToDeg(error), radToDeg(t), radToDeg(pose.theta));
    // calculate distance travelled
    dist = fabs(error);

    // calculate the speed
    // converts error to degrees to make PID tuning easier
    float output = turnPID.update(radToDeg(error), 0);
    printf(" O:%.2f\n", output);

    // cap the speed
    output = clamp(int(std::round(output)), -maxSpeed, maxSpeed);
    // return output
    return {output, -output};
}
