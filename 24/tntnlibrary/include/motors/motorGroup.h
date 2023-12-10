#pragma once

#include <functional>
#include <memory>
#include <vector> // Include this line
#include "vex.h"
#include "../tntnlibrary/include/defaultDevices.h"
#include "../tntnlibrary/include/motors/motor.h"

namespace tntnlib
{
    class MotorGroup : public Motor
    {
    public:
        /**
         * @brief Construct a new Rotation Encoder
         *
         * @param port the port of the rotation sensor
         * @param reversed true if the rotation sensor should be reversed, false otherwise
         * @param ratio the gear ratio to use. Input / Output
         */
        /**
         * @brief Construct a new Flywheel
         *
         * @param motors the motorgroup that makes up the flywheel
         * @param inputRPM the rpm of your motors (IE, 100, 200, 600)
         * @param outputRPM the rpm of the flywheel (IE 3000)
         */
        template <typename... Ports>
        MotorGroup(vex::gearSetting gear, float outputRPM, Ports... ports)
            : outputRPM(outputRPM)
        {
            if (gear == vex::gearSetting::ratio6_1)
            {
                inputRPM = 600;
            }
            else if (gear == vex::gearSetting::ratio18_1)
            {
                inputRPM = 200;
            }
            else if (gear == vex::gearSetting::ratio36_1)
            {
                inputRPM = 100;
            }
            else
            {
                inputRPM = 600;
            }
            std::array<int, sizeof...(Ports)> portsArray = {ports...};
            int size = portsArray.size();
            motors.resize(size, vex::motor(0));
            for (int i = 0; i < size; i++)
            {
                motors[i] = vex::motor(abs(portsArray[i]) - 1, gear, (portsArray[i] < 0 ? true : false));
                printf("Created motor on port %d\n", portsArray[i]);
            }
        }

        void spinRPM(double rpm) override;
        float getRPM() override;
        float getWatts() override;
        float getVolts() override;
        float getPower(float rpm) override;
        void spinVolts(float volts) override;

        float targetRPM = 0;
        float currentRPM = 0;
        std::vector<vex::motor> motors;

    private:
        double inputRPM = 0;
        double outputRPM = 0;
        float lastRPMEmaOutput = 0;
        float lastWattEmaOutput = 0;
        float kV = 0, kP = 0, kI = 0;
        float error = 0, lastError = 0, integral = 0;
        float bangBangMargin = 0;
    };
} // namespace tntnlib
