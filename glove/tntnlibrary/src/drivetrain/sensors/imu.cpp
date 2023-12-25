#include <cmath>
#include "../tntnlibrary/include/drivetrain/sensors/gyro/imu.h"
#include <iostream>
using namespace tntnlib;
/**
 * Construct a new vex Imu
 *
 * This is a class derived from the Gyro class
 */
Imu::Imu(int port, float multiplier)
    : inertial(vex::inertial(port - 1)),
      port(port),
      multiplier(multiplier)
{
}

/**
 * Get the angle the Imu rotated by, in degrees
 *
 * Pretty straightforward, raw value from the imu gets multiplied by a constant
 */
float Imu::rotation()
{
  return (inertial.rotation(vex::deg)) * multiplier;
}

/**
 * Set the imu to desired degrees
 */
void Imu::setTo(float deg)
{
  inertial.setRotation(deg, vex::deg);
}

void Imu::calibrate()
{
  inertial.calibrate();
}

bool Imu::isCalibrating()
{
  return inertial.isCalibrating();
}