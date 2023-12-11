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
#include "../tntnlibrary/include/drivetrain/devices/encoder/encoder.h"
#include "../tntnlibrary/include/motors/motorGroup.h"

namespace tntnlib {
class MotorEncoder : public Encoder {
    public:
        /**
         * @brief Construct a new Motor Encoder
         *
         * @note change to reference instead of pointer when PROS 4 releases
         *
         * @param motors pointer to the motor group to be used
         * @param rpm output rpm
         */
        MotorEncoder(MotorGroup* motors, float cartridge_rpm, float rpm);

        /**
         * @brief Get the angle rotated by the motor encoders, in radians
         *
         * @return float angle rotated by the motor encoders, in radians
         */
        float getAngle() override;
        /**
         * @brief Reset the motor encoder
         *
         * @return true calibration failed
         * @return false calibration succeeded
         */
        bool reset() override;
    private:
        MotorGroup* motors;
        const float rpm;
        const float cartridgeRpm;
};
} // namespace tntnlib
