#include <cmath>
#include <bits/stdc++.h>
#include "vex.h"
#include "tntnlib/util.h"
#include "tntnlib/pose.h"
#include "tntnlib/movements/moveTo.h"
#include "vex.h"

using namespace tntnlib;

bool MoveTo::useBoomerang = false;

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

std::pair<float, float> MoveTo::update(Pose pose)
{
    // set state to 1 if in state 0 and close to the target
    if (state == 0 && pose.distance(targetPose) < 4) {
        state = 1;
        if (!MoveTo::useBoomerang) {
            targetPose.theta = StandardFormRadToDeg(pose.angle(targetPose));
        }
    }


    // if going in reverse, flip the heading of the pose
    if (reversed)
        pose.theta += 180;

    float targetHeadingRad = degToStandardFormRad(targetPose.theta);
    float poseError = pose.distance(targetPose);
    // calculate the carrot point
    Pose carrot = targetPose - Pose(poseError * lead * cos(targetHeadingRad), poseError * lead * sin(targetHeadingRad));
    if (state == 1 or !MoveTo::useBoomerang)
        carrot = targetPose; // settling behavior

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
            linearPower = maxTurnSpeed;
        else if (linearPower < -maxTurnSpeed && state == 0)
            linearPower = -maxTurnSpeed;
    }

    // prioritize turning over moving
    float overturn = fabs(angularPower) + fabs(linearPower) - lmaxSpeed;
    if (overturn > 0)
        linearPower -= (linearPower > 0 ? overturn : -overturn);

    // calculate motor powers
    float leftPower = linearPower + angularPower;
    float rightPower = linearPower - angularPower;

    /*std::cout << "targetPose " << targetPose.x << " " << targetPose.y << " " << targetPose.theta << "\n"
              << "carrotPose " << carrot.x << " " << carrot.y << " " << carrot.theta << "\n"
              << "pose " << pose.x << " " << pose.y << " " << pose.theta << "\n";
    std::cout << "angleError " << angularError << " linearError " << linearError << "\n"
              << "\n";
    std::cout << "LP " << leftPower << " RP " << rightPower << "\n"
              << std::endl;
    std::cout << poseError << std::endl;
     std::cout << "(" << carrot.x << "," << carrot.y << ")," << std::endl;*/

    // return motor output
    breakOutError = poseError;
    return {leftPower, rightPower};
    // return {0, 0};
}
