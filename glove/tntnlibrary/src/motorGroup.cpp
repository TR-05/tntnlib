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

void MotorGroup::setDiameter(float diameter)
{
    this->diameter = diameter;
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
    lastRPMEmaOutput = ema(rawOutput, lastRPMEmaOutput, this->smoothing);
    currentRPM = lastRPMEmaOutput;
    return currentRPM;
}

float MotorGroup::getTipVelocity()
{
    return getRPM() * diameter * M_PI / 60;
}

float MotorGroup::voltsToTipVelocity(float volts)
{
    return (volts / kV) * outputRPM * diameter * M_PI / 60;
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

float MotorGroup::tipVelocityToRPM(float tipVelocity)
{
    return tipVelocity * 60 / (diameter * M_PI);
}

float MotorGroup::getPower(float rpm)
{
    float currentRPM = getRPM();
    rpmError = (rpm - currentRPM) / outputRPM;
    float kpPow = 0;
    //if (error > 0)
    kpPow = kP * rpmError;
    //test = kpPow;
    integral += rpmError;
    if (sgn(rpmError) != sgn(lastError))
    {
        integral /= 2;
    }
    lastError = rpmError;
    float kpguy = rpmError;
    if (fabs(rpmError) > .3)
        kpguy = 12 * sgn(rpmError);
    float power = (kV * rpm / outputRPM) + kpguy*kP + kI * integral;
    if (sgn(power) == sgn(currentRPM))
        power *= kAcc;
    else
        power *= kDec;
    //printf("\ntRPM: %.2f cRPM: %.2f v: %.2f kV: %.2fkI: %.2f e: %.4f kp: %.2f", rpm, currentRPM, power, (kV * rpm / outputRPM), integral*kI, rpmError, kpguy*kP);
    test = kI * integral;
    getVolts();
    clamp(power, -12, 12);
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

void MotorGroup::spinTipVelocity(float tipVelocity)
{
    targetRPM = tipVelocityToRPM(tipVelocity);
    if (targetRPM == 0)
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
