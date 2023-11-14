/**
 * @file include/tntnlib/devices/gyro.h
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
#include <cstdint>
#include "vex.h"
#include "tntnlib/devices/gyro/baseGyro.h"

namespace tntnlib {

class Gyro {
    public:
        /**
         * @brief Creates a new gyro using a vex inertial sensor on the specified port
         *
         * @param encoder unique ptr to the custom encoder to be used
         * @param diameter the diameter of the wheel, in inches
         * @param offset distance between the wheel and the tracking center, in inches
         */
        Gyro(int port, float multiplier);
   
        /**
         * @brief set the gyro to input degrees
         *
         */
        void setTo(float deg);
        /**
         * @brief Get the rotational angle of the gyro, in degrees
         *
         * @return float rotational angle of the gyro, in degrees
         */
        float rotation();
        /**
         * @brief Gets the gyro's constant multiplier
         *
         * @return float multiplier
         */
        float getMultiplier() const;

        void calibrate();

        bool isCalibrating();

    private:
        std::shared_ptr<BaseGyro> gyro;
        const float multiplier;
};
} // namespace tntnlib