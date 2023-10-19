/**
 * @file src/tntnlib/pid.cpp
 * @author Trevor Ruttan
 * @brief Pose class declarations
 * @version 0.0
 * @date 2023-10-12
 *
 * @copyright Copyright (c) 2023
 *
 */


#include <iostream>
#include <math.h>
#include "tntnlib/pid.h"
#include "tntnlib/util.h"


/**
 * @brief Construct a new FAPID
 *
 * @param kF feedfoward gain, multiplied by target and added to output. Set 0 if disabled
 * @param kA acceleration gain, limits the change in output. Set 0 if disabled
 * @param kP proportional gain, multiplied by error and added to output
 * @param kI integral gain, multiplied by total error and added to output
 * @param kD derivative gain, multiplied by change in error and added to output
 * @param name name of the FAPID. Used for logging
 */
tntnlib::FAPID::FAPID(float kF, float kA, float kP, float kI, float kD) {
    this->kF = kF;
    this->kA = kA;
    this->kP = kP;
    this->kI = kI;
    this->kD = kD;
}

/**
 * @brief Set gains
 *
 * @param kF feedfoward gain, multiplied by target and added to output. Set 0 if disabled
 * @param kA acceleration gain, limits the change in output. Set 0 if disabled
 * @param kP proportional gain, multiplied by error and added to output
 * @param kI integral gain, multiplied by total error and added to output
 * @param kD derivative gain, multiplied by change in error and added to output
 */
void tntnlib::FAPID::setGains(float kF, float kA, float kP, float kI, float kD) {
    this->kF = kF;
    this->kA = kA;
    this->kP = kP;
    this->kI = kI;
    this->kD = kD;
}

/**
 * @brief Set the exit conditions
 *
 * @param largeError
 * @param smallError
 * @param largeTime
 * @param smallTime
 * @param maxTime
 */
void tntnlib::FAPID::setExit(float largeError, float smallError, int largeTime, int smallTime, int maxTime) {
    this->largeError = largeError;
    this->smallError = smallError;
    this->largeTime = largeTime;
    this->smallTime = smallTime;
    this->maxTime = maxTime;
}

/**
 * @brief Update the FAPID
 *
 * @param target the target value
 * @param position the current value
 * @param log whether to check the most recent terminal input for user input. Default is false because logging multiple
 * PIDs could slow down the program.
 * @return float - output
 */
float tntnlib::FAPID::update(float target, float position, bool log) {
    // check most recent input if logging is enabled
    // this does not run by default because the mutexes could slow down the program
    // calculate output
    float error = target - position;
    float deltaError = error - prevError;
    float output = kF * target + kP * error + kI * totalError + kD * deltaError;
    if (kA != 0) output = tntnlib::slew(output, prevOutput, kA);
    prevOutput = output;
    prevError = error;
    totalError += error;

    return output;
}

/**
 * @brief Reset the FAPID
 */
void tntnlib::FAPID::reset() {
    prevError = 0;
    totalError = 0;
    prevOutput = 0;
}

/**
 * @brief Check if the FAPID has settled
 *
 * If the exit conditions have not been set, this function will always return false
 *
 * @return true - the FAPID has settled
 * @return false - the FAPID has not settled
 */
bool tntnlib::FAPID::settled() {
    float t = time(true);
    if (startTime == 0) { // if maxTime has not been set
        startTime = t;
        return false;
    } else { // check if the FAPID has settled
        if (t - startTime > maxTime) return true; // maxTime has been exceeded
        if (fabs(prevError) < largeError) { // largeError within range
            if (!largeTimeCounter) largeTimeCounter = t; // largeTimeCounter has not been set
            else if (t - largeTimeCounter > largeTime) return true; // largeTime has been exceeded
        }
        if (fabs(prevError) < smallError) { // smallError within range
            if (!smallTimeCounter) smallTimeCounter = t; // smallTimeCounter has not been set
            else if (t - smallTimeCounter > smallTime) return true; // smallTime has been exceeded
        }
        // if none of the exit conditions have been met
        return false;
    }
}

