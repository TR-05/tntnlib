#pragma once

#include "vex.h"
#include "../tntnlibrary/include/devices/trackingWheel.h"
#include "../tntnlibrary/include/devices/gyro.h"

namespace tntnlib
{

        /**
         * @brief Struct containing all the sensors used for odometry
         *
         */
        struct OdomSensors
        {
                /**
                 * The sensors are stored in a struct so that they can be easily passed to the chassis class
                 * The variables are pointers so that they can be set to nullptr if they are not used
                 * Otherwise the chassis class would have to have a constructor for each possible combination of sensors
                 *
                 * @param vertical1 pointer to the first vertical tracking wheel
                 * @param vertical2 pointer to the second vertical tracking wheel
                 * @param horizontal1 pointer to the first horizontal tracking wheel
                 * @param horizontal2 pointer to the second horizontal tracking wheel
                 * @param imu pointer to the IMU
                 */
                OdomSensors(TrackingWheel *vertical1, TrackingWheel *vertical2, TrackingWheel *horizontal1,
                            TrackingWheel *horizontal2, Gyro *imu)
                    : vertical1(vertical1),
                      vertical2(vertical2),
                      horizontal1(horizontal1),
                      horizontal2(horizontal2),
                      gyro(imu) {}

                TrackingWheel *vertical1;
                TrackingWheel *vertical2;
                TrackingWheel *horizontal1;
                TrackingWheel *horizontal2;
                Gyro *gyro;
        };

        /**
         * @brief Struct containing constants for a chassis controller
         *
         */
        struct ControllerSettings
        {
                /**
                 * @brief Struct containing constants for a chassis pid controller

                 * The constants are stored in a struct so that they can be easily passed to the chassis class
                 * Set a constant to 0 and it will be ignored
                 *
                 * @param kP the proportional constant
                 * @param kI the integral constant
                 * @param kD the derivative constant
                 * @param kImax the maximum integral value
                 * @param kIstart the minimum error to start integrating
                 * @param slew the maximum change in voltage per iteration
                 */
        
                ControllerSettings(float kP, float kI, float kD, float kImax, float kIstart, float slew)
                    : kP(kP),
                      kI(kI),
                      kD(kD),
                      kIStart(kIstart),
                      kIMax(kImax),
                      slew(slew) {}

                float kP;
                float kI;
                float kD;
                float kIStart;
                float kIMax;
                float slew;
        };

        /**
         * @brief Struct containing constants for a drivetrain
         *
         * The constants are stored in a struct so that they can be easily passed to the chassis class
         * Set a constant to 0 and it will be ignored
         *
         * @param leftMotors pointer to the left motors
         * @param rightMotors pointer to the right motors
         * @param trackWidth the track width of the robot
         * @param wheelDiameter the diameter of the wheel used on the drivetrain
         * @param rpm the rpm of the wheels
         * @param chasePower higher values make the robot move faster but causes more overshoot on turns
         */
        struct Drivetrain
        {
                /**
                 * The constants are stored in a struct so that they can be easily passed to the chassis class
                 * Set a constant to 0 and it will be ignored
                 *
                 * @param leftMotors shared pointer to the left motors
                 * @param rightMotors shared pointer to the right motors
                 * @param trackWidth the track width of the robot
                 * @param wheelDiameter the diameter of the wheel used on the drivetrain
                 * @param rpm the rpm of the wheels
                 * @param chasePower higher values make the robot move faster but causes more overshoot on turns
                 */
                Drivetrain(vex::motor_group *leftMotors, vex::motor_group *rightMotors,
                           float trackWidth, float wheelDiameter, float rpm, float chasePower)
                    : leftMotors(leftMotors),
                      rightMotors(rightMotors),
                      trackWidth(trackWidth),
                      wheelDiameter(wheelDiameter),
                      rpm(rpm),
                      chasePower(chasePower) {}

                vex::motor_group *leftMotors;
                vex::motor_group *rightMotors;
                float trackWidth;
                float wheelDiameter;
                float rpm;
                float chasePower;
        };

        extern ControllerSettings linearSettings;
        extern ControllerSettings angularSettings;
        extern Drivetrain drivetrain;
        extern OdomSensors sensors;
} // namespace tntnlib