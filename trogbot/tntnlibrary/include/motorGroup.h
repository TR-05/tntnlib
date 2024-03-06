#pragma once

#include <functional>
#include <memory>
#include <vector> // Include this line
#include "vex.h"
#include "../tntnlibrary/include/defaultDevices.h"

namespace tntnlib
{
    class MotorGroup
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
        void initializeVeloController(float kV, float kP, float kI, float kAcc, float kDec, float bangBangMargin, float integralMargin);
        void setBrakeType(vex::brakeType type);
        void setDiameter(float diameter);
        void spinVolts(float volts);
        void spinPct(float pct);
        void stop(vex::brakeType type);
        void spinRPM(double rpm);
        float getRPM();
        float getTipVelocity();
        float getWatts();
        float getCurrent();
        float getVolts();
        float getPower(float rpm);
        float position();
        void resetPosition();
        void driverTwoButton(bool in, bool out, float inVolts, float outVolts);
        void driverToggle(bool input, float inVolts);

        float targetRPM = 0;
        float currentRPM = 0;
        float rpmError = 0;
        float test = 0;
        std::vector<vex::motor> motors;
    private:
        float diameter = 0;
        float inputRPM = 0;
        float outputRPM = 0;
        float lastRPMEmaOutput = 0;
        float lastWattEmaOutput = 0;
        float kV = 11, kP = 0, kI = 0, kAcc = 1, kDec = 1;
        float error = 0, lastError = 0, integral = 0;
        float bangBangMargin = 2.0, integralMargin = 0; //bangBangMargin initalizes to never activate unless changed by user
        float currentVoltage = 0;

        bool lastToggleInput = false;
        vex::brakeType brakeType = vex::brakeType::coast;
    };
} // namespace tntnlib
