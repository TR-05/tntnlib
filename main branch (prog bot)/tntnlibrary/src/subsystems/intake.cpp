#include <math.h>
#include "vex.h"
#include "../tntnlibrary/include/util.h"
#include "../tntnlibrary/include/pid.h"
#include "../tntnlibrary/include/subsystems/intake.h"
#include <functional>
using namespace tntnlib;

/**
 * @brief Get the current Intake RPM scaled from 0 to 1
 *
 * @return float rpm
 */
float Intake::getRPM()
{
    double total = 0;
    float motorCount = 0;
    for (auto &motor : motors)
    {
        total += motor.velocity(vex::velocityUnits::rpm);
        motorCount++;
    }
    double rawOutput = total / motorCount;
    double stableVelo = rawOutput / inputRPM;
    rawOutput = rawOutput * (outputRPM / inputRPM); // converts to output RPM
    lastRPMEmaOutput = ema(stableVelo, lastRPMEmaOutput, .2);
    return lastRPMEmaOutput;
}

float Intake::getWatts()
{
    double total = 0;
    float motorCount = 0;
    for (vex::motor &motor : motors)
    {
        total += motor.power(vex::powerUnits::watt);
        motorCount++;
    }
    double rawOutput = total / motorCount;
    return rawOutput;
}

float Intake::getVolts()
{
    double total = 0;
    float motorCount = 0;
    for (vex::motor &motor : motors)
    {
        total += motor.voltage(vex::voltageUnits::volt);
        motorCount++;
    }
    double rawOutput = total / motorCount;
    return rawOutput;
}

void Intake::setBrakeType(vex::brakeType type)
{
    brakeType = type;
}

void Intake::stop()
{
    for (auto &motor : motors)
    {
        motor.stop(brakeType);
    }
}

void Intake::spinVolts(float volts)
{
    for (auto &motor : motors)
    {
        motor.spin(vex::directionType::fwd, volts, vex::voltageUnits::volt);
    }
}

void Intake::spinPct(float pct)
{
    for (auto &motor : motors)
    {
        motor.spin(vex::directionType::fwd, pct, vex::velocityUnits::pct);
    }
}

void Intake::driver(bool in, bool out, float inVolts, float outVolts)
{
    if (in)
    {
        spinVolts(inVolts);
    }
    else if (out)
    {
        spinVolts(outVolts);
    }
    else
    {
        stop();
    }
}

void Intake::driverToggle(bool input, float inVolts)
{
    if (input && !lastToggleInput)
    {
        if (intakeVolts == inVolts)
        {
            intakeVolts = 0;
        }
        else
        {
            intakeVolts = inVolts;
        }
    }
    lastToggleInput = input;
    if (intakeVolts != 0)
    spinVolts(intakeVolts);
    else stop();
}