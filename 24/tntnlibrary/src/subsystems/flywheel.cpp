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
        // task = make_unique<vex::task>(FlywheelLoop);
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

/**
 * @brief Get the current flywheel RPM scaled from 0 to 1
 *
 * @return float rpm
 */
float Flywheel::getRPM()
{
    double total = 0;
    float motorCount = 0;
    for (auto &motor : *motors)
    {
        total += motor.velocity(vex::velocityUnits::rpm);
        motorCount++;
    }
    double rawOutput = total / motorCount;
    double stableVelo = rawOutput / inputRPM;
    rawOutput = rawOutput * (outputRPM / inputRPM); // converts to output RPM
    lastRPMEmaOutput = ema(stableVelo, lastRPMEmaOutput, .2);
    currentRPM = lastRPMEmaOutput * 3600;
    return lastRPMEmaOutput;
}

float Flywheel::getWatts()
{
    double total = 0;
    float motorCount = 0;
    for (vex::motor &motor : *motors)
    {
        total += motor.power(vex::powerUnits::watt);
        motorCount++;
    }
    double rawOutput = total / motorCount;
    return rawOutput;
}

float Flywheel::getVolts()
{
    double total = 0;
    float motorCount = 0;
    for (vex::motor &motor : *motors)
    {
        total += motor.voltage(vex::voltageUnits::volt);
        motorCount++;
    }
    double rawOutput = total / motorCount;
    return rawOutput;
}

void Flywheel::spinVolts(float volts)
{
    for (auto &motor : *motors)
    {
        motor.spin(vex::directionType::fwd, volts, vex::voltageUnits::volt);
    }
}

float Flywheel::getPower(float rpm)
{
    error = targetRPM - getRPM();
    if (error > bangBangMargin)
    {
        return 12;
    }
    else
    {
        integral += error;
        if (sgn(error) != sgn(lastError) && sgn(error) == 1)
        {
            integral /= 2;
        }
        lastError = error;
        float power = clamp( kV * targetRPM + kP * error + kI * integral, -12, 12);
        //printf("power: %.2f integral: %.2f kI: %.2f rpm: %.3f\n", power, integral, kI, getRPM());
        return power; 
    }
}

/**
 * @brief Spins the flywheel at a given RPM
 *
 * @param rpm new rpm value to spin at
 *
 */
void Flywheel::spinRPM(double rpm)
{
    targetRPM = rpm / outputRPM;
    spinVolts(getPower(targetRPM));
}