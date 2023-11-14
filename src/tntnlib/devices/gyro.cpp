/**
 * @file src/tntnlib/devices/trackingWheel.cpp
 * @author Trevor Ruttan
 * @brief Pose class declarations
 * @version 0.0
 * @date 2023-10-12
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <math.h>
#include "tntnlib/util.h"
#include "tntnlib/devices/gyro.h"
#include "tntnlib/devices/gyro/imu.h"

using namespace tntnlib;
/**
 * @brief Create a Gyro object using a vex inertial sensor
 *
 * @param port the brain port the inertial is plugged into
 * @param multiplier the output multiplier to use
 */
Gyro::Gyro(int port, float multiplier)
    : gyro(std::make_shared<Imu>(port, multiplier)),
      multiplier(multiplier) {}

void Gyro::setTo(float deg)
{
    gyro->setTo(deg);
}

void Gyro::calibrate() { gyro->calibrate(); }

bool Gyro::isCalibrating() { return gyro->isCalibrating(); }

float Gyro::getMultiplier() const { return multiplier; }

float Gyro::rotation()
{
    return gyro->rotation();
}
