/**
 * @file include/tntnlib/devices/trackingWheel.h
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
#include "../tntnlibrary/include/motors/motorGroup.h"

namespace tntnlib
{
    class Motor
    {
    public:
        /**
         * @brief Create a tracking wheel with a custom encoder
         *
         * @param encoder unique ptr to the custom encoder to be used
         * @param diameter the diameter of the wheel, in inches
         * @param offset distance between the wheel and the tracking center, in inches
         */
        template <typename... Ports>
        Motor(vex::gearSetting gear, float outputRPM, Ports... ports);

        virtual void spinRPM(double rpm) = 0;
        virtual float getRPM() = 0;
        virtual float getWatts() = 0;
        virtual float getVolts() = 0;
        virtual float getPower(float rpm) = 0;
        virtual void spinVolts(float volts) = 0;

    private:
        std::shared_ptr<MotorGroup> motors;
        const float diameter;
        const float offset;
    };
} // namespace tntnlib