#include <cmath>
#include "troglib/devices/encoder/optical.h"
#include <iostream>
#include "troglib/devices/defaultDevices.h"
/**
 * Construct a new Optical Encoder
 *
 * This is a derived class of the Encoder abstract class.
 * This abstraction is easy as there is only a single sensor which needs to be
 * checked.
 */
// vex::encoder testEncoder = vex::encoder(Brain.ThreeWirePort.A);

// troglib::OpticalEncoder::OpticalEncoder(vex::triport::port port, float ratio)
//: optical(port),
//  ratio(ratio) {}
troglib::OpticalEncoder::OpticalEncoder(vex::triport::port &port, float ratio)
    : optical(port),
      ratio(ratio) {}

/**
 * Get the angle the rotation sensor rotated by, in radians
 *
 * Pretty straightforward, raw value from the rotation sensor gets converted to rotations
 * which gets converted to radians
 */

float troglib::OpticalEncoder::getAngle()
{
  return (optical.position(vex::deg)) * (2 * M_PI) / ratio / 360;
}

/**
 * Reset/calibrate the optical encoder
 */

bool troglib::OpticalEncoder::reset()
{
  optical.resetRotation();
    return 0;
}
