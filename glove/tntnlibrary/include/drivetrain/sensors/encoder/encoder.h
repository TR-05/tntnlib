#pragma once

#include "vex.h"

namespace tntnlib
{
    class Encoder
    {
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
} // namespace tntnlib
