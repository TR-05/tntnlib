/**
 * @file include/tntnlib/devices/motor.h
 * @author Trevor Ruttan
 * @brief Pose class declarations
 * @version 0.0
 * @date 2023-10-12
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <memory>
#include "vex.h"
#include "../tntnlibrary/include/drivetrain/devices/gyro/basegyro.h"

namespace tntnlib
{
    class Imu : public BaseGyro
    {
    public:
        /**
         * @brief Construct a new Vex Inertial sensor
         *
         * @param port the brain port the inertial sensor is plugged into
         * @param multiplier the multiplier to use for the output rotation
         */
        Imu(int port, float multiplier);

        /**
         * @brief Get the angle rotated by the imu, in degrees
         *
         * @return float angle rotated by the imu, in degrees
         */
        float rotation() override;
        /**
         * @brief Set the imu to desired degrees
         */
        void setTo(float deg) override;

        /**
         * @brief calibrate the imu
         */
        void calibrate() override;

        /**
         * @brief Checks if the imu is calibrating
         *
         * @return bool true if the imu is calibrating, false otherwise
         */
        bool isCalibrating() override;

    private:
        vex::inertial inertial;
        int port;
        const float multiplier;
    };
} // namespace tntnlib
