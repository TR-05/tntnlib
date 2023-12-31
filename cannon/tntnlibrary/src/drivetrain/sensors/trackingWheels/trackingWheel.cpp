#include <math.h>
#include "../tntnlibrary/include/util.h"
#include "../tntnlibrary/include/drivetrain/sensors/trackingWheel.h"
#include "../tntnlibrary/include/drivetrain/sensors/encoder/motor.h"
#include "../tntnlibrary/include/drivetrain/sensors/encoder/optical.h"
#include "../tntnlibrary/include/drivetrain/sensors/encoder/rotation.h"
/**
 * @brief Create a tracking wheel with a custom encoder
 *
 * @param encoder unique ptr to the custom encoder to be used
 * @param diameter the diameter of the wheel, in inches
 * @param offset distance between the wheel and the tracking center, in inches
 */
tntnlib::TrackingWheel::TrackingWheel(std::shared_ptr<Encoder> encoder, float diameter, float offset)
    : encoder(encoder),
      diameter(diameter),
      offset(offset) {}

/**
 * Construct a new motor encoder tracking wheel
 *
 * We pass a pointer to a motor group instead of a reference motor group due to a
 * limitation of PROS 3. This is fixed in PROS 4, but its not ready for release yet
 */
tntnlib::TrackingWheel::TrackingWheel(MotorGroup * motors, float diameter, float offset,  float cartridgeRpm, float rpm)
    : encoder(new MotorEncoder(motors, cartridgeRpm, rpm)),
      diameter(diameter),
      offset(offset) {}

/**
 * Construct a new optical encoder tracking wheel
 *
 * We let the user the option to pass the ports and reversal bool directly, which
 * means they don't have to construct 2 objects for 1 tracking wheel
 */

tntnlib::TrackingWheel::TrackingWheel(vex::triport::port &port, float diameter, float offset, float ratio)
    : encoder(std::make_shared<OpticalEncoder>(port, ratio)),
      diameter(diameter),
      offset(offset) {}

/**
 * Construct a new rotation sensor tracking wheel
 *
 * We let the user pass in a signed integer for the port. Negative ports is a short form which
 * means that the rotation sensor should be reversed.
 */
tntnlib::TrackingWheel::TrackingWheel(int port, float diameter, float offset, float ratio)
    : encoder(std::make_shared<RotationEncoder>(port, ratio)),
      diameter(diameter),
      offset(offset) {}

/**
 * Reset the tracking wheel.
 */
bool tntnlib::TrackingWheel::reset() { encoder->reset(); 
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
float tntnlib::TrackingWheel::getDistance() { 
  return encoder->getAngle() / 2 * diameter; }

/**
 * Get the offset from the tracking center, in inches
 */
float tntnlib::TrackingWheel::getOffset() const { return this->offset; }

/**
 * Get the diameter of the wheel, in inches
 */
float tntnlib::TrackingWheel::getDiameter() const { return this->diameter; }
