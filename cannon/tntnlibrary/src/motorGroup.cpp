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
    double rawOutput = (total / motorCount) * (outputRPM / 100.0);
    lastRPMEmaOutput = ema(rawOutput, lastRPMEmaOutput, 1.0);
    currentRPM = lastRPMEmaOutput;
    return currentRPM;
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

float MotorGroup::getCurrent()
{
    double total = 0;
    float motorCount = 0;
    for (auto &motor : motors)
    {
        total += motor.current(vex::amp);
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
       // printf("%.2f ", motor.voltage(vex::voltageUnits::volt));
        motorCount++;
    }
    //printf("\n");
    double rawOutput = total / motorCount;
    return rawOutput;
}

float MotorGroup::getPower(float rpm)
{
    rpmError = (rpm - getRPM()) / outputRPM;
    float kpPow = 0;
    //if (error > 0)
    kpPow = kP * rpmError;
    //test = kpPow;
    integral += rpmError;
    if (sgn(rpmError) != sgn(lastError) && sgn(rpmError) == 1)
    {
        integral /= 2;
    }
    lastError = rpmError;
    float kpguy = rpmError;
    if (sgn(kpguy) == -1)
        kpguy = 0;
    if (rpmError > .1)
        kpguy = 12;
    float power = (kV * rpm / outputRPM) + kpguy*0 + kI * integral;
    if (sgn(power) == sgn(getRPM()))
        power *= kAcc;
    else
        power *= kDec;
    //printf("\n\ncurRpm: %.2f power: %.2f integral: %.2f kI: %.2f error: %.3f\n", rpm, power, integral, kI, error);
    test = kI * integral;
    getVolts();
    return power;
    // }
}
void MotorGroup::setBrakeType(vex::brakeType type)
{
    brakeType = type;
}

void MotorGroup::stop(vex::brakeType type)
{
    for (auto &motor : motors)
    {
        motor.setMaxTorque(.1, vex::amp);
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
            motor.setMaxTorque(2.5, vex::amp);
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
    targetRPM = rpm;
    if (rpm == 0)
    {
        stop(brakeType);
    }
    else
    {
        spinVolts(getPower(targetRPM));
    }
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
