/**
 * @file include/troglib/devices/encoder.h
 * @author Trevor Ruttan
 * @brief Pose class declarations
 * @version 0.0
 * @date 2023-10-12
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "vex.h"

namespace troglib {
class Encoder {
    public:
        /**
         * @brief Construct a new Encoder
         *
         */
        Encoder() {}

        /**
         * @brief Get the angle rotated by the encoder, in radians
         *
         * @return float angle rotated by the encoder, in radians
         */
        virtual float getAngle() = 0;
        /**
         * @brief Reset the encoder
         *
         * @return true encoder calibration failed
         * @return false encoder calibration succeeded
         */
        virtual bool reset() = 0;
};
} // namespace troglib
