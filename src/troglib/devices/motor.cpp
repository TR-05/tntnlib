#include <cmath>
#include "troglib/util.h"
#include "troglib/devices/encoder/motor.h"

/**
 * Construct a new motor encoder object
 *
 * This is a derived class of the Encoder abstract class.
 * Currently, a pointer to a MotorGroup is passed instead of just using
 * a reference, due to limitations in PROS 3. This is fixed in PROS 4, but
 * we have to deal with this for now.
 */
troglib::MotorEncoder::MotorEncoder(vex::motor_group *motors, float cartridgeRpm, float rpm)
    : motors(motors),
      rpm(rpm),
      cartridgeRpm(cartridgeRpm) {}

/**
 * Get the angle the motors rotated by, in radians
 *
 * Since the motors in the group may have different cartridges, we need some
 * extra logic to calculate the geared output. All we do is get a vector
 * of all the gearboxes, and calculate the appropriate ratio by diving the
 * output rpm by the input rpm. Then we just multiply the output by 2 pi
 * to get angle in radians.
 */
float troglib::MotorEncoder::getAngle()
{
    // get gearboxes and encoder position for each motor in the group
    int cartridge = this->cartridgeRpm;
    double position = this->motors->position(vex::deg);
    // calculate ratio'd output for each motor
    float angle = (position * (rpm / cartridge) * 2 * M_PI);

    // return average of elements in the angles vector
    return (angle);
}

/**
 * Reset the motor encoders.
 */
bool troglib::MotorEncoder::reset()
{
    motors->resetPosition();
    return 0;
}
