/**
 * @file src/lemlib/chassis/chassis.cpp
 * @author LemLib Team
 * @brief definitions for the chassis class
 * @version 0.4.5
 * @date 2023-01-27
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <math.h>
#include "vex.h"
#include "../tntnlibrary/include/util.h"
#include "../tntnlibrary/include/pid.h"
#include "../tntnlibrary/include/subsystems/flywheel/flywheel.h"
#include <functional>

using namespace tntnlib;

/**
 * Initialize the chassis
 *
 * Calibrates sensors and starts the chassis task
 */

// make_unique c++11 implementation
template <class T, class... Args>
std::unique_ptr<T> make_unique(Args &&...args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

int Flywheel::FlywheelLoop()
{
    while (1)
    {
        this->update();
        vex::wait(10, vex::msec);
    }
    return 0;
}

void Flywheel::initialize()
{
    this->spinRPM(0);
    // start the chassis task if it doesn't exist
    if (task == nullptr)
    {
        task = make_unique<vex::task>(FlywheelLoop);
        printf("new chassis thead\n");
    }
    else
    {
        printf("thread already exists\n");
    }
}
        /**
         * @brief Set the pose of the chassis
         *
         * @param rpm new rpm value to spin at
         *
         */
        void Flywheel::spinRPM(float rpm) 
        {

        }
        /**
         * @brief Get the current flywheel RPM
         *
         * @return float rpm
         */
        float Flywheel::getRPM() 
        {

        }
        void Flywheel::settings(float kV, float kP, float kI, float kD, float bangBangMargin) 
        {

        }

        void Flywheel::spinVolts(float volts)
        {

        }

        void Flywheel::update()
        {

        }

        void Flywheel::stateMachineOn()
        {

        }

        void Flywheel::stateMachineOff()
        {

        }