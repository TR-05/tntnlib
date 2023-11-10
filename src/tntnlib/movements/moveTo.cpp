#include <cmath>
#include <bits/stdc++.h>
#include "vex.h"
#include "tntnlib/util.h"
#include "tntnlib/pose.h"
#include "tntnlib/movements/moveTo.h"
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

void MoveTo::params(Pose target, bool reversed, float lmaxSpeed, float amaxSpeed, float lead, float chasePower)
{
    targetPose = target;
    MoveTo::reversed = reversed;
    MoveTo::lmaxSpeed = lmaxSpeed;
    MoveTo::amaxSpeed = amaxSpeed;
    MoveTo::lead = lead;
    MoveTo::chasePower = chasePower;
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
/*
std::pair<float, float> tntnlib::MoveTo::update(Pose pose)
{
    float t;
    if (useHeading)
        t = targetHeading;
    else
    {
        t = StandardFormRadToDeg(pose.angle(targetPose));
    }

    t = fmod(t, 360);

    // reverse heading if doing movement in reverse
    if (reversed)
    {
        pose.theta = fmod(pose.theta - 180, 360);
    }

    // update completion vars
    if (dist == 0)
    { // if dist is 0, this is the first time update() has been called
        dist = 0.0001;
        startPose = pose;
    }

    // calculate error
    float error = angleError(t, pose.theta, false);
    // calculate distance travelled
    dist = fabs(error);

    // calculate the speed
    // converts error to degrees to make PID tuning easier
    float output = angularPID.update(error, 0);
    // cap the speed
    output = clamp(output, -maxSpeed, maxSpeed);

    // return output
    if (swingOnLeft)
        return {0, -output};
    if (swingOnRight)
        return {output, 0};
    return {output, -output};
}
*/
std::pair<float, float> tntnlib::MoveTo::update(Pose pose)
{
    // set state to 1 if in state 0 and close to the target
    if (state == 0 && pose.distance(targetPose) < 7.5)
        state = 1;

    // if going in reverse, flip the heading of the pose
    if (reversed)
        pose.theta += 180;

    // update completion vars
    if (dist == 0)
    { // if dist is 0, this is the first time update() has been called
        dist = 0.0001;
        prevPose = pose;
    }
    dist += pose.distance(prevPose);
    prevPose = pose;

    float targetHeadingRad = degToStandardFormRad(targetPose.theta);
    float currentHeadingRad = degToStandardFormRad(pose.theta);
    float distance = pose.distance(targetPose);
    // calculate the carrot point
    Pose carrot = targetPose - Pose(distance * lead * cos(targetHeadingRad), distance * lead * sin(targetHeadingRad));
    if (state == 1)
        carrot = targetPose; // settling behavior

    // calculate error
    float angularError = angleError(pose.angle(carrot), degToRad(pose.theta), true); // angular error
    float linearError = pose.distance(carrot) * cos(angularError);   // linear error
    std::cout << "targetPose " << targetPose.x << " " << targetPose.y << " " << targetPose.theta << "\n"
     << "carrotPose " << carrot.x << " " << carrot.y << " " << carrot.theta << "\n"
     << "pose " << pose.x << " " << pose.y << " " << pose.theta << "\n";
    std::cout << "angleError " << radToDeg(angularError) << " " << linearError << "\n" << std::endl;

    if (state == 1)
        angularError = angleError(targetPose.theta, currentHeadingRad); // settling behavior
    if (reversed)
        linearError = -linearError;

    // get PID outputs
    float angularPower = angularPID.update(StandardFormRadToDeg(angularError), 0);
    float linearPower = linearPID.update(linearError, 0);

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
            linearPower = maxTurnSpeed;
        else if (linearPower < -maxTurnSpeed && state == 0)
            linearPower = -maxTurnSpeed;
    }

    // prioritize turning over moving
    float overturn = fabs(angularPower) + fabs(linearPower) - lmaxSpeed;
    if (overturn > 0)
        linearPower -= linearPower > 0 ? overturn : -overturn;

    // calculate motor powers
    float leftPower = linearPower + angularPower;
    float rightPower = linearPower - angularPower;

    // return motor output
    //return {leftPower, rightPower};
    return {0, 0};
}