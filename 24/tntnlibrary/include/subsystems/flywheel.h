#pragma once

#include <functional>
#include <memory>

#include "vex.h"

#include "../tntnlibrary/include/defaultDevices.h"

namespace tntnlib
{
    /**
     * @brief Chassis class
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
        Flywheel(std::vector<vex::motor> *motors, float inputRPM, float outputRPM, float kV, float kP, float kI, float bangBangMargin)
            : motors(motors),
              inputRPM(inputRPM),
              outputRPM(outputRPM),
              kV(kV),
              kP(kP),
              kI(kI),
              bangBangMargin(bangBangMargin)
        {
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

        std::vector<vex::motor> *motors;

        double targetRPM = 0;
        double currentRPM = 0;
    private:
        bool StateMachineEnabled = false;
        float prevDist = 0; // the previous distance travelled by the movement
        const double inputRPM = 0;
        const double outputRPM = 0;
        float kV = 0, kP = 0, kI = 0;
        float error = 0, lastError = 0, integral = 0;
        float lastRPMEmaOutput = 0;
        float lastWattEmaOutput = 0;
        float bangBangMargin = 0;
        std::unique_ptr<vex::task> task;
    };
} // namespace tntnlib