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
        Flywheel(vex::motor_group motors, float inputRPM, float outputRPM)
            : motors(motors),
              inputRPM(inputRPM),
              outputRPM(outputRPM)
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
        void spinRPM(float rpm);
        /**
         * @brief Get the current flywheel RPM
         *
         * @return float rpm
         */
        float getRPM();
        void settings(float kV, float kP, float kI, float kD, float bangBangMargin);

        void spinVolts(float volts);

        void update();

        void stateMachineOn();

        void stateMachineOff();

        float stateMachinePower = 0;

    private:
        bool StateMachineEnabled = false;
        float prevDist = 0; // the previous distance travelled by the movement
        const float inputRPM = 0;
        const float outputRPM = 0;
        std::unique_ptr<vex::task> task;
        vex::motor_group motors;
    };
} // namespace tntnlib