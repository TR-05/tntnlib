/**
 * @file include/tntnlib/chassis/chassis.hpp
 * @author tntnlib Team
 * @brief Chassis class declarations
 * @version 0.4.5
 * @date 2023-01-23
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include <functional>
#include <memory>

#include "vex.h"

#include "../tntnlibrary/include/drivetrain/pose.h"
#include "../tntnlibrary/include/drivetrain/devices/trackingWheel.h"
#include "../tntnlibrary/include/drivetrain/devices/gyro.h"
#include "../tntnlibrary/include/drivetrain/chassis/structs.h"
#include "../tntnlibrary/include/drivetrain/chassis/odom.h"
#include "../tntnlibrary/include/defaultDevices.h"
#include "../tntnlibrary/include/drivetrain/pathing/cubicBezier.h"

namespace tntnlib
{

    /**
     * @brief  Default drive curve. Modifies  the input with an exponential curve. If the input is 127, the function
     * will always output 127, no matter the value of scale, likewise for -127. This curve was inspired by team 5225, the
     * Pilons. A Desmos graph of this curve can be found here: https://www.desmos.com/calculator/rcfjjg83zx
     * @param input value from -127 to 127
     * @param scale how steep the curve should be.
     * @return The new value to be used.
     */
    float defaultDriveCurve(float input, float scale);

    /**
     * @brief Chassis class
     *
     */
    class Chassis
    {
        friend class Odometry;

    public:
        /**
         * @brief Construct a new Chassis
         *
         * @param drivetrain drivetrain to be used for the chassis
         * @param lateralSettings settings for the lateral controller
         * @param angularSettings settings for the angular controller
         * @param sensors sensors to be used for odometry
         * @param driveCurve drive curve to be used. defaults to `defaultDriveCurve`
         */
        Chassis(Drivetrain drivetrain, ControllerSettings linearSettings, ControllerSettings angularSettings,
                OdomSensors sensors)
            : drivetrain(drivetrain), linearSettings(linearSettings), angularSettings(angularSettings), sensors(sensors), odom(sensors)
        {
        }

        /**
         * @brief Initialize the chassis
         *
         * @param calibrateIMU whether to calibrate the IMU. True by default
         */
        void initialize(bool calibrateIMU, float x, float y, float theta);

        /**
         * @brief Set the pose of the chassis
         *
         * @param x new x value
         * @param y new y value
         * @param theta new theta value
         * @param radians true if theta is in radians, false if not. False by default
         */
        void setPose(float x, float y, float theta, bool radians = false);

        /**
         * @brief Set the pose of the chassis
         *
         * @param pose the new pose
         * @param radians whether pose theta is in radians (true) or not (false). false by default
         */
        void setPose(Pose pose, bool radians = false);

        /**
         * @brief Get the pose of the chassis
         *
         * @param radians whether theta should be in radians (true) or degrees (false). false by default
         * @return Pose
         */
        Pose getPose(bool radians = false);

        Pose getOffsetPose();
        void setOffset(float x, float y);
        /**
         * @brief Wait until the robot has traveled a certain distance, or angle, along the path
         *
         * @note Units are in inches if current motion is moveTo or follow, degrees if using turnTo
         *
         * @param dist the distance the robot needs to travel before returning
         */
        void waitUntilDist(float dist);

        /**
         * @brief Wait until the current movement is done
         *
         */
        void waitUntilDone();

        /**
         * @brief Wait until the robot has an error less than a threshold
         *
         * @note Units of error and margin must match
         *
         * @param error the changing variable that you are measuring
         *
         * @param margin the static margin
         */
        void waitUntilError(float &error, float margin);

        void moveToSettings(float akp, float aki, float akd, float lkp, float lki, float lkd, float slew);

        /**
         * @brief Prepare the angular pid for a turn movement
         *
         * The PID logging id is "angularPID"
         *
         */
        void turnSettings(float kp, float ki, float kd);

        /**
         * @brief Turn the chassis so it is facing a point
         *
         * The PID logging id is "angularPID"
         *
         * @param x x location
         * @param y y location
         * @param reversed whether the robot should turn to face the point with the back of the robot. false by default
         * @param maxSpeed the maximum speed the robot can turn at
         * @param kp the turn kp value
         * @param ki the turn ki value
         * @param kd the turn kd value
         * @param breakAngle the error value to break out of the movement at

         */
        void turnToPose(float x, float y, bool reversed, float maxSpeed, float kp, float ki, float kd, float breakAngle);
        void SwingOnLeftToPose(float x, float y, bool reversed, float maxSpeed, float kp, float ki, float kd, float breakAngle);
        void SwingOnRightToPose(float x, float y, bool reversed, float maxSpeed, float kp, float ki, float kd, float breakAngle);

        /**
         * @brief Turn the chassis so it is facing a heading
         *
         * The PID logging id is "angularPID"
         *
         * @param heading the heading the robot should face. Units are in degrees
         * @param reversed whether the robot should turn to face the point with the back of the robot. false by default
         * @param maxSpeed the maximum speed the robot can turn at
         * @param kp the turn kp value
         * @param ki the turn ki value
         * @param kd the turn kd value
         * @param breakAngle the error value to break out of the movement at
         */
        void turnToHeading(float heading, bool reversed, float maxSpeed, float kp, float ki, float kd, float breakAngle);
        void turnToHeadingUnbounded(float heading, bool reversed, float maxSpeed, float kp, float ki, float kd, float breakAngle);
        void tuneOffsets(float ang, float kp, float ki, float kd, float maxSpeed, float breakang);
        void SwingOnLeftToHeading(float heading, bool reversed, float maxSpeed, float kp, float ki, float kd, float breakAngle);
        void SwingOnRightToHeading(float heading, bool reversed, float maxSpeed, float kp, float ki, float kd, float breakAngle);

        void pid(float dist, float heading, bool reversed, float lmaxSpeed, float amaxSpeed, float lkp, float lki, float lkd, float akp, float aki, float akd, float slew, float breakDist);
        void pid(float dist, float x, float y, bool reversed, float lmaxSpeed, float amaxSpeed, float lkp, float lki, float lkd, float akp, float aki, float akd, float slew, float breakDist);
        /**
         * @brief Move the chassis towards the target pose
         *
         * Uses the boomerang controller
         *
         * @param x x location
         * @param y y location
         * @param theta theta (in degrees). Target angle
         * @param reversed whether the robot should move forwards or backwards. true for forwards (default), false for
         * backwards
         * @param chasePower higher values make the robot move faster but causes more overshoot on turns. 0 makes it
         * default to global value
         * @param lead the lead parameter. Determines how curved the robot will move. 0.6 by default (0 < lead < 1)
         *
         * @param maxSpeed the maximum speed the robot can move at. 12 at default
         */
        void boomerangTo(float x, float y, float theta, bool reversed, float lmaxSpeed, float amaxSpeed, float lkp, float lki, float lkd, float akp, float aki, float akd, float chasePower, float lead, float slew, float breakDist);

        /**
         * @brief Move the chassis towards the target pose
         *
         * Uses the moveTo controller
         *
         * @param x x location
         * @param y y location
         * @param reversed whether the robot should move forwards or backwards. true for forwards (default), false for backwards
         * @param lmaxSpeed the maximum linear speed the robot can move at
         * @param amaxSpeed the maximum angular speed the robot can move at
         * @param lk the gain for the linear PID controller
         * @param ak the gain for the angular PID controller
         * @param slew the slew rate for the robot's movement
         * @param breakDist the breaking distance for the robot
         */
        void moveTo(float x, float y, bool reversed, float lmaxSpeed, float amaxSpeed, float lkp, float lki, float lkd, float akp, float aki, float akd, float slew, float breakDist);

        /**
         * @brief Move the chassis along a path
         *
         * @param path the precomputed cubic curve to follow
         * @param reversed whether the robot should follow the path in reverse. false by default
         * @param lmaxSpeed the maximum linear speed the robot can move at
         * @param amaxSpeed the maximum angular speed the robot can move at
         * @param lkp the proportional gain for the linear PID controller
         * @param lki the integral gain for the linear PID controller
         * @param lkd the derivative gain for the linear PID controller
         * @param akp the proportional gain for the angular PID controller
         * @param aki the integral gain for the angular PID controller
         * @param akd the derivative gain for the angular PID controller
         * @param slew the slew rate for the robot's movement
         * @param lookAhead the look-ahead distance for the path follower
         * @param breakDist the breaking distance for the robot
         */
        void follow(Path &path, bool reversed, float lmaxSpeed, float amaxSpeed, float lkp, float lki, float lkd, float akp, float aki, float akd, float slew, float lookAhead, float breakDist);

        void autoTankVolts(float left, float right);
        void autoTankPct(float left, float right);

        /**
         * @brief Control the robot during the driver control period using the tank drive control scheme. In this
         * control scheme one joystick axis controls one half of the robot, and another joystick axis controls another.
         * @param left speed of the left side of the drivetrain. Takes an input from -100 to 100.
         * @param right speed of the right side of the drivetrain. Takes an input from -100 to 100.
         * @param curveGain control how steep the drive curve is. The larger the number, the steeper the curve. A value
         * of 0 disables the curve entirely.
         */
        void tank(int left, int right, float curveGain = 0.0);

        /**
         * @brief Control the robot during the driver using the arcade drive control scheme. In this control scheme one
         * joystick axis controls the forwards and backwards movement of the robot, while the other joystick axis
         * controls  the robot's turning
         * @param throttle speed to move forward or backward. Takes an input from -100 to 100.
         * @param turn speed to turn. Takes an input from -100 to 100.
         * @param curveGain the scale inputted into the drive curve function. If you are using the default drive
         * curve, refer to the `defaultDriveCurve` documentation.
         */
        void arcade(int throttle, int turn, float curveGain = 0.0);

        /**
         * @brief Control the robot during the driver using the curvature drive control scheme. This control scheme is
         * very similar to arcade drive, except the second joystick axis controls the radius of the curve that the
         * drivetrain makes, rather than the speed. This means that the driver can accelerate in a turn without changing
         * the radius of that turn. This control scheme defaults to arcade when forward is zero.
         * @param throttle speed to move forward or backward. Takes an input from -100 to 100.
         * @param turn speed to turn. Takes an input from -100 to 100.
         * @param curveGain the scale inputted into the drive curve function. If you are using the default drive
         * curve, refer to the `defaultDriveCurve` documentation.
         */
        void curvature(int throttle, int turn, float cureGain = 0.0);
        void update();
        void stateMachineOn();
        void stateMachineOff();
        enum moveState
        {
            disabledMode,
            turnMode,
            moveToMode,
            followMode,
            drivePidMode,
            voltageMode,
            velocityMode
        };
        moveState autoChassis = disabledMode;
        enum motorMoveType
        {
            voltage,
            velocity
        };
        motorMoveType motorControl = voltage;
        std::pair<float, float> stateMachine();

        ControllerSettings linearSettings;
        ControllerSettings angularSettings;
        Drivetrain drivetrain;
        OdomSensors sensors;

    private:
        /**
         * @brief Chassis update function. Updates chassis motion and odometry
         *
         */
        bool StateMachineEnabled = false;
        float leftVolts = 0, rightVolts = 0;
        Pose offsetPose = Pose(0, 0, 0);
        float prevDist = 0; // the previous distance travelled by the movement
        Odometry odom;
        // std::unique_ptr<Movement> movement;
        std::unique_ptr<vex::task> task;
    };
} // namespace tntnlib
extern tntnlib::Chassis chassis;
