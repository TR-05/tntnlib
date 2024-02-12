#pragma once

#include "vex.h"

namespace tntnlib {
class BaseGyro {
    public:
        /**
         * @brief Construct a new Gyro object
         *
         */
        BaseGyro() {}

        /**
         * @brief Get the rotational angle of the gyro, in degrees
         *
         * @return float rotational angle of the gyro, in degrees
         */
        virtual float rotation() = 0;

        virtual void calibrate() = 0;

        virtual bool isCalibrating() = 0;

        /**
         * @brief set the gyro to input degrees
         *
         */
        virtual void setTo(float deg) = 0;

};
} // namespace tntnlib
