#pragma once

#include <vector>

#include "../tntnlibrary/include/statemachine/motorGroup.h"

namespace tntnlib
{
namespace stateMachine
{
/**
 * @brief Struct containing constants for a drivetrain
 *
 * The constants are stored in a struct so that they can be easily passed to the chassis class
 * Set a constant to 0 and it will be ignored
 *
 * @param leftMotors pointer to the left motors
 * @param rightMotors pointer to the right motors
 * @param trackWidth the track width of the robot
 * @param wheelDiameter the diameter of the wheel used on the drivetrain
 * @param rpm the rpm of the wheels
 * @param chasePower higher values make the robot move faster but causes more overshoot on turns
 */
struct Drivetrain
{
    Drivetrain(MotorGroup *leftMotors, MotorGroup *rightMotors,
               float trackWidth, float wheelDiameter, float rpm)
        : leftMotors(leftMotors),
          rightMotors(rightMotors),
          trackWidth(trackWidth),
          wheelDiameter(wheelDiameter),
          rpm(rpm)
    {
    }

    MotorGroup *leftMotors;
    MotorGroup *rightMotors;
    float trackWidth;
    float wheelDiameter;
    float rpm;
};

std::vector<MotorGroup> robotMotors;
}  // namespace stateMachine
}  // namespace tntnlib