#include "../tntnlibrary/include/drivetrain/chassis/chassis.h"
#include "../tntnlibrary/include/util.h"
#include "vex.h"
#include <math.h>

namespace tntnlib {

/**
 * @brief  Trevors custom curve, takes an input from +-100 and returns a curved value from +-12
 * set gain to 1, min to 0, fullstick to 12, and deadband to 0 for a linear curve
 * recommended values are gain:3 min:1, fullstick:12, deadband:3
 * @param input value from -127 to 127
 * @param gain how steep the curve should be.  1-4 is a good range for this value.
 * @param min the minimum value to be outputted. This gives more control over the lower end of the curve.
 * @param fullStick at what percent stick should it output max. 80-100 is a good range for this value.
 * @param deadband the deadband of the input in percent. 0-10 is a good range for this value.
 * @return The new value to be used.
 */
float sherbertCurve(float input, float gain, float min, float fullStick, float deadband) {
    input = input / 12.0;
    int sign = sgn(input);
    if (input*100.0/12.0 < deadband && input*100.0/12.0 > -deadband) {
        return 0;
    }
    float output = (sign * (powf(fabs(input), gain)) / (powf(fullStick/12.0, gain - 1)) ) + min*sign;
    if (fabs(output) > 12.0) {
        return 12.0 * sign;
    }
    return output;
}


/**
 * @brief Control the robot during the driver using the arcade drive control scheme. In this control scheme one
 * joystick axis controls the forwards and backwards movement of the robot, while the other joystick axis
 * controls  the robot's turning
 * @param throttle speed to move forward or backward. Takes an input from -12 to 12.
 * @param turn speed to turn. Takes an input from -12 to 12.
 * @param curveGain the scale inputted into the drive curve function. If you are using the default drive
 * curve, refer to the `defaultDriveCurve` documentation.
 */
void Chassis::tank(int left, int right, float gain, float min, float fullStick, float deadband) {
    drivetrain.leftMotors->spinVolts(sherbertCurve(left, gain, min, fullStick, deadband));
    drivetrain.rightMotors->spinVolts(sherbertCurve(right, gain, min, fullStick, deadband));
}

/**
 * @brief Control the robot during the driver using the arcade drive control scheme. In this control scheme one
 * joystick axis controls the forwards and backwards movement of the robot, while the other joystick axis
 * controls  the robot's turning
 * @param throttle speed to move forward or backward. Takes an input from -12 to 12.
 * @param turn speed to turn. Takes an input from -12 to 12.
 * @param curveGain the scale inputted into the drive curve function. If you are using the default drive
 * curve, refer to the `defaultDriveCurve` documentation.
 */
void Chassis::arcade(int throttle, int turn, float gain, float min, float fullStick, float deadband) {
    drivetrain.leftMotors->spinVolts(sherbertCurve(throttle+turn, gain, min, fullStick, deadband));
    drivetrain.rightMotors->spinVolts(sherbertCurve(throttle-turn, gain, min, fullStick, deadband));
}
} // namespace tntnlib
