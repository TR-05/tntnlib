#pragma once
#include <memory>
#include "vex.h"
#include "../tntnlibrary/include/sensing/sensors/encoder/encoder.h"
#include "../tntnlibrary/include/statemachine/motorGroup.h"

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
