/**
 * @file include/tntnlib/devices/rotation.h
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
#include "../tntnlibrary/include/drivetrain/devices/encoder/encoder.h"

namespace tntnlib {
class RotationEncoder : public Encoder {
    public:
        /**
         * @brief Construct a new Rotation Encoder
         *
         * @param port the port of the rotation sensor
         * @param reversed true if the rotation sensor should be reversed, false otherwise
         * @param ratio the gear ratio to use. Input / Output
         */
        RotationEncoder(int port, float ratio);

        /**
         * @brief Get the angle rotated by the rotation sensor, in radians
         *
         * @return float angle rotated by the rotation sensor, in radians
         */
        float getAngle() override;
        /**
         * @brief Reset the rotation sensor
         *
         * @return true calibration failed
         * @return false calibration succeeded
         */
        bool reset() override;
    private:
        vex::rotation rotation;
        const float ratio;
};
} // namespace tntnlib
