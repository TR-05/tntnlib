#pragma once

#include <vector>

#include "../tntnlibrary/include/statemachine/motorGroup.h"

namespace tntnlib
{
namespace stateMachine
{

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

template <typename... MotorGroup>
void initialize(MotorGroup... groups)
{
    std::array<MotorGroups*, sizeof...(MotorGroups)> groupsArray = {&groups...};
    int size = groupsArray.size();
    robotMotors.resize(size);
    for (int i = 0; i < size; i++)
    {
        robotMotors[i] = groupsArray[i]; 
        printf("Created %s on slot %d\n", robotMotors[i]->getName().c_str(), i);
    }
}

std::vector<MotorGroup*> robotMotors;
}  // namespace stateMachine
}  // namespace tntnlib