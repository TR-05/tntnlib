#pragma once
 
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
     * @param kImax maximum total error
     * @param kIstart minimum error to start integrating
     * @param slew maximum change in output per update
     */
    FAPID(float kF, float kA, float kP, float kI, float kD, float kImax, float kIstart, float slew);

    void setGains(float kF, float kA, float kP, float kI, float kD, float kImax, float kIstart, float slew);

    float update(float target, float position);
    /**
     * @brief Reset the FAPID
     */
    void reset();

    float error = 0, prevError = 0;

   private:
    float kF, kA, kP, kI, kD, kIStart, kIMax, slew;
    float totalError = 0, prevOutput = 0;
};

extern FAPID angularPID;
extern FAPID linearPID;

}  // namespace tntnlib
