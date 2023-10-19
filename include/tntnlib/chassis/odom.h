/**
 * @file include/lemlib/chassis/odom.hpp
 * @author LemLib Team
 * @brief This is the header file for the odom.cpp file. Its not meant to be used directly, only through the chassis
 * class
 * @version 0.4.5
 * @date 2023-01-23
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include "tntnlib/pose.h"
#include "tntnlib/chassis/structs.h"

namespace tntnlib {
class Odometry {
    public:
        /**
         * @brief Construct a new Odometry object
         *
         * @param chassis pointer to the chassis object
         */
        Odometry(OdomSensors_t& sensors, Drivetrain_t& drive)
            : sensors(sensors),
              drive(drive) {}

        /**
         * @brief Calibrate sensors used for odometry
         *
         * @param calibrateIMU whether to calibrate the imu.
         */
        void calibrate(bool calibrateIMU);

        /**
         * @brief Get the Pose
         *
         * @note Units are in radians. Locked from 0 to 2pi. Right is 0, increasing counter-clockwise
         *
         * @return Pose
         */
        Pose getPose();

        /**
         * @brief Set the Pose
         *
         * @note Units are in radians. Right is 0, increasing counter-clockwise
         *
         * @param pose what the pose should be set to
         */
        void setPose(Pose pose);

        /**
         * @brief Update the pose of the robot
         *
         * @note this should be called in a loop, ideally every 10ms (the sensor polling rate)
         */
        void update();
    private:
        OdomSensors_t sensors;
        Drivetrain_t drive;
        Pose pose = Pose(0, 0, 0);

        float prevVertical = 0;
        float prevVertical1 = 0;
        float prevVertical2 = 0;
        float prevHorizontal = 0;
        float prevHorizontal1 = 0;
        float prevHorizontal2 = 0;
        float prevImu = 0;
};
} // namespace tntnlib