#include "../tntnlibrary/include/drivetrain/chassis/chassis.h"
#include "vex.h"
#include <math.h>

namespace tntnlib {

/**
 * @brief  Default drive curve. Modifies the input with an exponential curve. If the input is 127, the function
 * will always output 127, no matter the value of scale, likewise for -127. A scale of zero disable the curve
 * entirely. This curve was inspired by team 5225, the Pilons. A Desmos graph of this curve can be found
 * here: https://www.desmos.com/calculator/rcfjjg83zx
 * @param input value from -127 to 127
 * @param scale how steep the curve should be.
 * @return The new value to be used.
 */
float defaultDriveCurve(float input, float scale) {
    if (scale != 0) {
        return (powf(2.718, ( ( (fabs(input) - 12) * scale ) / 1000.0)) * input);
    }
    return input;
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
void Chassis::tank(int left, int right, float curveGain) {
    drivetrain.leftMotors->spinVolts(defaultDriveCurve(left, curveGain));
    drivetrain.rightMotors->spinVolts(defaultDriveCurve(right, curveGain));
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
void Chassis::arcade(int throttle, int turn, float curveGain) {
    drivetrain.leftMotors->spinVolts(defaultDriveCurve(throttle+turn, curveGain));
    drivetrain.rightMotors->spinVolts(defaultDriveCurve(throttle-turn, curveGain));
}

/**
 * @brief Control the robot during the driver using the curvature drive control scheme. This control scheme is
 * very similar to arcade drive, except the second joystick axis controls the radius of the curve that the
 * drivetrain makes, rather than the speed. This means that the driver can accelerate in a turn without changing
 * the radius of that turn. This control scheme defaults to arcade when forward is zero.
 * @param throttle speed to move forward or backward. Takes an input from -127 to 127.
 * @param turn speed to turn. Takes an input from -127 to 127.
 * @param curveGain the scale inputted into the drive curve function. If you are using the default drive
 * curve, refer to the `defaultDriveCurve` documentation.
 */
void Chassis::curvature(int throttle, int turn, float curveGain) {
    // If we're not moving forwards change to arcade drive
    if (throttle == 0) {
        arcade(throttle, turn, curveGain);
        return;
    }

    float leftPower = throttle + (abs(throttle) * turn) / 127.0;
    float rightPower = throttle - (abs(throttle) * turn) / 127.0;

    leftPower = defaultDriveCurve(leftPower, curveGain);
    rightPower = defaultDriveCurve(rightPower, curveGain);

    drivetrain.leftMotors->spinVolts(leftPower);
    drivetrain.rightMotors->spinVolts(rightPower);
}
} // namespace tntnlib
