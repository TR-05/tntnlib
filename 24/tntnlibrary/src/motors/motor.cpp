#include <math.h>
#include "../tntnlibrary/include/util.h"
#include "../tntnlibrary/include/motors/motor.h"

using namespace tntnlib;


Motor::Motor(vex::gearSetting gear, float outputRPM, int port)
    : MotorGroup(gear, outputRPM, port)
{
}
/**
 * Reset the tracking wheel.
 */
bool TrackingWheel::reset() { encoder->reset(); 
return (encoder->getAngle() == 0);
}

/**
 * Get the distance travelled by the tracking wheel, in inches
 *
 * Since we get angle in radians, but need to convert to inches, we can simplify
 * the calculation. So, instead of writing
 * (angle / (2 * pi)) * pi * diameter
 * we do
 * (angle / 2) * diameter
 */
float TrackingWheel::getDistance() { 
  return encoder->getAngle() / 2 * diameter; }

/**
 * Get the offset from the tracking center, in inches
 */
float TrackingWheel::getOffset() const { return this->offset; }

/**
 * Get the diameter of the wheel, in inches
 */
float TrackingWheel::getDiameter() const { return this->diameter; }
