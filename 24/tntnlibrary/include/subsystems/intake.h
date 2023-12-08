#pragma once

#include <functional>
#include <memory>
#include "vex.h"
#include "../tntnlibrary/include/defaultDevices.h"

namespace tntnlib
{
    /**
     * @brief Intake class
     *
     */
    class Intake
    {

    public:
        /**
         * @brief Construct a new Intake
         *
         * @param motors the motorgroup that makes up the Intake
         * @param inputRPM the rpm of your motors (IE, 100, 200, 600)
         * @param outputRPM the rpm of the Intake (IE 3000)
         */
        template <typename... Ports>
        Intake(vex::gearSetting gear, float outputRPM, Ports... ports)
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
            motors.resize(size, vex::motor(-1));
            for (int i = 0; i < size; i++)
            {
                motors[i] = vex::motor(abs(portsArray[i]) - 1, gear, (portsArray[i] < 0 ? true : false));
                printf("Created motor on port %d\n", portsArray[i]);
            }
        }

        /**
         * @brief Get the current Intake RPM
         *
         * @return float rpm
         */
        float getRPM();
        float getWatts();
        float getVolts();
        void setBrakeType(vex::brakeType type);
        void spinVolts(float volts);
        void spinPct(float pct);
        void stop();
        void driver(bool in, bool out, float inVolts, float outVolts);
        void driverToggle(bool input, float inVolts);

        double targetRPM = 0;
        std::vector<vex::motor> motors;

    private:
        double inputRPM = 600;
        const double outputRPM = 0;
        float lastWattEmaOutput = 0;
        float lastRPMEmaOutput = 0;
        bool lastToggleInput = false;
        float intakeVolts = 0;
        vex::brakeType brakeType = vex::brakeType::coast;
    };
} // namespace tntnlib