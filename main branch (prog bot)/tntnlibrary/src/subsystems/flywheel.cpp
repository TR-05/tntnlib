/**
 * @file src/lemlib/chassis/chassis.cpp
 * @author LemLib Team
 * @brief definitions for the chassis class
 * @version 0.4.5
 * @date 2023-01-27
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <math.h>
#include "vex.h"
#include "../tntnlibrary/include/util.h"
#include "../tntnlibrary/include/pid.h"
#include "../tntnlibrary/include/subsystems/flywheel.h"
#include <functional>
using namespace tntnlib;

template <class T, class... Args>
std::unique_ptr<T> make_unique(Args &&...args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

int Flywheel::FlywheelLoop()
{
    while (1)
    {
        this->update();
        vex::wait(10, vex::msec);
    }
    return 0;
}

void Flywheel::initialize()
{
    this->spinRPM(0);
    // start the chassis task if it doesn't exist
    if (task == nullptr)
    {
        //task = make_unique<vex::task>(FlywheelLoop);
        printf("new chassis thead\n");
    }
    else
    {
        printf("thread already exists\n");
    }
}

void Flywheel::stateMachineOn()
{
}

void Flywheel::stateMachineOff()
{
}

void Flywheel::settings(float kV, float kA, float kP, float kI, float kD, float bangBangMargin)
{
    flywheelPID.setGains(kV, kA, kP, kI, kD);
    this->bangBangMargin = bangBangMargin;
}

/**
 * @brief Get the current flywheel RPM
 *
 * @return float rpm
 */
float Flywheel::getRPM()
{
    float rpmTotal = 0;
    int motorCount = 0;
    for (auto &motor : *motors)
    {
        // Your code here
        rpmTotal += motor.velocity(vex::velocityUnits::rpm);
        motorCount++;
    }
    float rawRPM = rpmTotal / motorCount;
    rawRPM = rawRPM * (outputRPM / inputRPM); // converts to output RPM
    lastEmaOutput = ema(rawRPM, lastEmaOutput, .5);
    return lastEmaOutput;
}

void Flywheel::spinVolts(float volts)
{
    for (auto &motor : *motors)
    {
        motor.spin(vex::directionType::fwd, volts, vex::voltageUnits::volt);
    }
}

float Flywheel::getTBHPower(float rpm)
{
    return 0;
}
float Flywheel::getFAPIDPower(float rpm)
{
    return flywheelPID.update(rpm, getRPM());
}

float Flywheel::getPower(float rpm)
{
    float tbh = getTBHPower(rpm);
    float fapid = getFAPIDPower(rpm);
    return tbh + fapid;
}

/**
 * @brief Spins the flywheel at a given RPM
 *
 * @param rpm new rpm value to spin at
 *
 */
void Flywheel::spinRPM(float rpm)
{
    float error = rpm - getRPM();
    if (error > bangBangMargin)
    {
        spinVolts(12);
    }
    else
    {
        spinVolts(getPower(getRPM()));
    }
}