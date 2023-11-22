#pragma once

#include "vex.h"
#include "../tntnlibrary/include/devices/trackingWheel.h"
#include "../tntnlibrary/include/devices/gyro.h"

namespace tntnlib
{
        /**
         * @brief Struct containing all the sensors used for odometry
         *
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
        struct OdomSensorStruct
        {
                TrackingWheel *vertical1;
                TrackingWheel *vertical2;
                TrackingWheel *horizontal1;
                TrackingWheel *horizontal2;
                Gyro *imu;
                void setUp(TrackingWheel *v1, TrackingWheel *v2, TrackingWheel *h1, TrackingWheel *h2, Gyro *i)
                {
                        vertical1 = v1;
                        vertical2 = v2;
                        horizontal1 = h1;
                        horizontal2 = h2;
                        imu = i;
                }
        };
        inline OdomSensorStruct OdomSensors;
        /**
         * @brief Struct containing constants for a chassis controller
         *
         * The constants are stored in a struct so that they can be easily passed to the chassis class
         * Set a constant to 0 and it will be ignored
         *
         * @param kP proportional constant for the chassis controller
         * @param kD derivative constant for the chassis controller
         * @param smallError the error at which the chassis controller will switch to a slower control loop
         * @param smallErrorTimeout the time the chassis controller will wait before switching to a slower control loop
         * @param largeError the error at which the chassis controller will switch to a faster control loop
         * @param largeErrorTimeout the time the chassis controller will wait before switching to a faster control loop
         * @param slew the maximum acceleration of the chassis controller
         */
        struct PIDSettings
        {
                float kP;
                float kI;
                float kD;
                float kIStart;
                float kIMax;
                float smallError;
                float smallErrorTimeout;
                float largeError;
                float largeErrorTimeout;
                float slew;
                void setUp(float p, float i, float d, float iStart, float iMax, float small, float smallTimeout, float large, float largeTimeout, float s)
                {
                        kP = p;
                        kI = i;
                        kD = d;
                        kIStart = iStart;
                        kIMax = iMax;
                        smallError = small;
                        smallErrorTimeout = smallTimeout;
                        largeError = large;
                        largeErrorTimeout = largeTimeout;
                        slew = s;
                }
        };
        inline PIDSettings AngularSettings;
        inline PIDSettings LinearSettings;

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
        struct DrivetrainStruct
        {
                vex::motor_group *leftMotors;
                vex::motor_group *rightMotors;
                float trackWidth;
                float wheelDiameter;
                float cartridgeRPM;
                float rpm;
                float chasePower;
                void setUp(vex::motor_group *leftMotors, vex::motor_group *rightMotors, float t, float w, float c, float r, float p)
                {
                        this->leftMotors = leftMotors;
                        this->rightMotors = rightMotors;
                        this->trackWidth = t;
                        this->wheelDiameter = w;
                        this->cartridgeRPM = c;
                        this->rpm = r;
                        this->chasePower = p;
                }
        };
        inline DrivetrainStruct DriveTrain;
        // extern tntnlib::OdomSensors_t sensors;
        // extern ChassisController_t linearController;
        // extern ChassisController_t angularController;
        // extern tntnlib::Drivetrain_t drivebase;

} // namespace tntnlib