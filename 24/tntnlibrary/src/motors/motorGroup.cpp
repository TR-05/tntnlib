#include <cmath>
#include "../tntnlibrary/include/motors/motorGroup.h"
#include <iostream>
#include "../tntnlibrary/include/defaultDevices.h"
#include "../tntnlibrary/include/util.h"
#include "../tntnlibrary/include/pid.h"

using namespace tntnlib;

/**
 * @brief Get the current Motor RPM scaled from 0 to 1
 *
 * @return float rpm
 */
float MotorGroup::getRPM()
{
    double total = 0;
    float motorCount = 0;
    for (auto &motor : motors)
    {
        total += motor.velocity(vex::percentUnits::pct);
        motorCount++;
    }
    double rawOutput = total / motorCount;
    double stableVelo = rawOutput / inputRPM;
    rawOutput = rawOutput * (outputRPM / inputRPM); // converts to output RPM
    lastRPMEmaOutput = ema(stableVelo, lastRPMEmaOutput, .2);
    currentRPM = lastWattEmaOutput;
    return lastRPMEmaOutput;
}

float MotorGroup::getWatts()
{
    double total = 0;
    float motorCount = 0;
    for (auto &motor : motors)
    {
        total += motor.power(vex::powerUnits::watt);
        motorCount++;
    }
    double rawOutput = total / motorCount;
    return rawOutput;
}

float MotorGroup::getVolts()
{
    double total = 0;
    float motorCount = 0;
    for (auto &motor : motors)
    {
        total += motor.voltage(vex::voltageUnits::volt);
        motorCount++;
    }
    double rawOutput = total / motorCount;
    return rawOutput;
}

void MotorGroup::spinVolts(float volts)
{
    for (auto &motor : motors)
    {
        motor.spin(vex::directionType::fwd, volts, vex::voltageUnits::volt);
    }}

float MotorGroup::getPower(float rpm)
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
        printf("power: %.2f integral: %.2f kI: %.2f rpm: %.3f\n", power, integral, kI, getRPM());
        return power; 
    }
}

/**
 * @brief Spins the Motor at a given RPM
 *
 * @param rpm new rpm value to spin at
 *
 */
void MotorGroup::spinRPM(double rpm)
{
    targetRPM = rpm / outputRPM;
    spinVolts(getPower(targetRPM));
}