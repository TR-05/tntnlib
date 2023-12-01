/**
 * @file include/tntnlib/pid.h
 * @author Trevor Ruttan
 * @brief Pose class declarations
 * @version 0.0
 * @date 2023-10-12
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <string>

namespace tntnlib
{
    /**
     * @brief Feedforward, Acceleration, Proportional, Integral, Derivative PID controller
     *
     * The controller does not loop on its own. It must be called in a loop.
     * For example: while(!controller.settled) { controller.update(input, output); }
     *
     */

    class FAPID
    {
    public:
        /**
         * @brief Construct a new FAPID
         *
         * @param kF feedfoward gain, multiplied by target and added to output. Set 0 if disabled
         * @param kA acceleration gain, limits the change in output. Set 0 if disabled
         * @param kP proportional gain, multiplied by error and added to output
         * @param kI integral gain, multiplied by total error and added to output
         * @param kD derivative gain, multiplied by change in error and added to output
         */
        FAPID(float kF, float kA, float kP, float kI, float kD);
        /**
         * @brief Set gains
         *
         * @param kF feedfoward gain, multiplied by target and added to output. Set 0 if disabled
         * @param kA acceleration gain, limits the change in output. Set 0 if disabled
         * @param kP proportional gain, multiplied by error and added to output
         * @param kI integral gain, multiplied by total error and added to output
         * @param kD derivative gain, multiplied by change in error and added to output
         */
        void setGains(float kF, float kA, float kP, float kI, float kD);
        /**
         * @brief Set the exit conditions
         *
         * @param largeError range where error is considered large
         * @param smallError range where error is considered small
         * @param largeTime time in milliseconds t
         * @param smallTime
         * @param maxTime
         */
        void setExit(float largeError, float smallError, int largeTime, int smallTime, int maxTime);

        void setIntegral(float kIStart, float kIMax);
        /**
         * @brief Update the FAPID
         *
         * @param target the target value
         * @param position the current value
         * @param log whether to check the most recent terminal input for user input. Default is false because logging
         * multiple PIDs could slow down the program.
         * @return float - output
         */
        float update(float target, float position, bool log = false);
        /**
         * @brief Reset the FAPID
         */
        void reset();
        /**
         * @brief Check if the FAPID has settled
         *
         * If the exit conditions have not been set, this function will always return false
         *
         * @return true - the FAPID has settled
         * @return false - the FAPID has not settled
         */
        bool settled();

        float prevError = 0;

    private:
        float kF;
        float kA;
        float kP;
        float kI;
        float kD;
        float kIStart;
        float kIMax;
        float slew;
        float largeError;
        float smallError;
        int largeTime = 0;
        int smallTime = 0;
        int maxTime = -1; // -1 means no max time set, run forever

        int largeTimeCounter = 0;
        int smallTimeCounter = 0;
        int startTime = 0;

        float totalError = 0;
        float prevOutput = 0;
    };


inline FAPID angularPID(0,0,0,0,0);
inline FAPID linearPID(0,0,0,0,0);
inline FAPID flywheelPID(0,0,0,0,0);
inline FAPID liftPID(0,0,0,0,0);

} // namespace tntnlib

