#include <cmath>
#include "../tntnlibrary/include/drivetrain/devices/encoder/rotation.h"

/**
 * Construct a new Rotation Encoder
 *
 * This is a class derived from the Encoder class.
 * This abstraction is pretty easy because there is only 1 sensor that needs
 * to be checked.
 */
tntnlib::RotationEncoder::RotationEncoder(int port, float ratio)
    : rotation(vex::rotation(port - 1, (ratio < 0))),
      ratio(ratio) {}

/**
 * Get the angle the rotation sensor rotated by, in radians
 *
 * Pretty straightforward, raw value from the rotation sensor gets converted to rotations
 * which gets converted to radians
 */
float tntnlib::RotationEncoder::getAngle() { return (float(rotation.position(vex::deg))) * (2 * M_PI) / ratio; }

/**
 * Reset/calibrate the optical encoder
 */
bool tntnlib::RotationEncoder::reset()
{
  rotation.resetPosition();
    return 0;
}
