#include <cmath>
#include "../tntnlibrary/include/motorGroup.h"
#include <iostream>
#include "../tntnlibrary/include/defaultDevices.h"
#include "../tntnlibrary/include/util.h"
#include "../tntnlibrary/include/pid.h"

using namespace tntnlib;

void MotorGroup::initializeVeloController(float kV, float kP, float kI, float kAcc, float kDec, float bangBangMargin, float integralMargin)
{
    this->kV = kV;
    this->kP = kP;
    this->kI = kI;
    this->kAcc = kAcc;
    this->kDec = kDec;
    this->bangBangMargin = bangBangMargin;
    this->integralMargin = integralMargin;
}

float MotorGroup::position()
{
    double total = 0;
    float motorCount = 0;
    for (auto &motor : motors)
    {
        total += motor.position(vex::rotationUnits::deg);
        motorCount++;
    }
    double rawOutput = total / motorCount;
    return rawOutput;
}

void MotorGroup::resetPosition()
{
    for (auto &motor : motors)
    {
        motor.resetPosition();
    }
}


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
        float power = kV * targetRPM + kP * error + kI * integral;
        if (sgn(power) == sgn(getRPM()))
            power *= kAcc;
        else
            power *=kDec;
        power = clamp(power, -12, 12);
        printf("power: %.2f integral: %.2f kI: %.2f rpm: %.3f\n", power, integral, kI, getRPM());
        return power;
    }
}

void MotorGroup::setBrakeType(vex::brakeType type)
{
    brakeType = type;
}

void MotorGroup::stop(vex::brakeType type)
{
    for (auto &motor : motors)
    {
        motor.stop(brakeType);
    }
}

void MotorGroup::spinVolts(float volts)
{
    for (auto &motor : motors)
    {
        if (volts == 0)
        {
            motor.stop(brakeType);
        }
        else
        {
            motor.spin(vex::directionType::fwd, volts, vex::voltageUnits::volt);
        }
    }
}

void MotorGroup::spinPct(float pct)
{
    for (auto &motor : motors)
    {
        if (pct == 0)
        {
            motor.stop(brakeType);
        }
        else
        {
            motor.spin(vex::directionType::fwd, pct, vex::velocityUnits::pct);
        }
    }
}

void MotorGroup::spinRPM(double rpm)
{
    targetRPM = rpm / outputRPM;
    spinVolts(getPower(targetRPM));
}

void MotorGroup::driverTwoButton(bool in, bool out, float inVolts, float outVolts)
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
        stop(brakeType);
    }
}

void MotorGroup::driverToggle(bool input, float inVolts)
{
    if (input && !lastToggleInput)
    {
        if (currentVoltage == inVolts)
        {
            currentVoltage = 0;
        }
        else
        {
            currentVoltage = inVolts;
        }
    }
    lastToggleInput = input;
    if (currentVoltage != 0)
        spinVolts(currentVoltage);
    else
        stop(brakeType);
}
