#pragma once

#include <functional>
#include <memory>
#include "vex.h"
#include "../tntnlibrary/include/defaultDevices.h"

namespace tntnlib
{
    /**
     * @brief Flywheel class
     *
     */
    class Flywheel
    {

    public:
        /**
         * @brief Construct a new Flywheel
         *
         * @param motors the motorgroup that makes up the flywheel
         * @param inputRPM the rpm of your motors (IE, 100, 200, 600)
         * @param outputRPM the rpm of the flywheel (IE 3000)
         */
        template <typename... Ports>
        Flywheel(vex::gearSetting gear, float outputRPM, float kV, float kP, float kI, float bangBangMargin, Ports... ports)
            : outputRPM(outputRPM),
              kV(kV),
              kP(kP),
              kI(kI),
              bangBangMargin(bangBangMargin)
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
        int FlywheelLoop();
        void initialize();
        /**
         * @brief Set the pose of the chassis
         *
         * @param rpm new rpm value to spin at
         *
         */
        void spinRPM(double rpm);
        /**
         * @brief Get the current flywheel RPM
         *
         * @return float rpm
         */
        float getRPM();
        float getWatts();
        float getVolts();

        float getPower(float rpm);
        void spinVolts(float volts);

        void update();

        void stateMachineOn();

        void stateMachineOff();

        float stateMachinePower = 0;

        std::vector<vex::motor> motors;

        float targetRPM = 0;
        float currentRPM = 0;

    private:
        bool StateMachineEnabled = false;
        float prevDist = 0; // the previous distance travelled by the movement
        double inputRPM = 0;
        const double outputRPM = 0;
        float kV = 0, kP = 0, kI = 0;
        float error = 0, lastError = 0, integral = 0;
        float lastRPMEmaOutput = 0;
        float lastWattEmaOutput = 0;
        float bangBangMargin = 0;
        std::unique_ptr<vex::task> task;
    };
} // namespace tntnlib