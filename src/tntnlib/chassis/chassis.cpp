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
#include "tntnlib/util.h"
#include "tntnlib/pid.h"
 #include "tntnlib/movements/moveTo.h"
#include "tntnlib/movements/purepursuit.h"
#include "tntnlib/movements/turn.h"
#include "tntnlib/chassis/chassis.h"
#include "tntnlib/chassis/odom.h"
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

int odomLoop()
{
    while (1)
    {
        chassis.update();
        vex::wait(10, vex::msec);
    }
    return 0;
}

void Chassis::initialize(bool calibrateIMU, float x, float y, float theta)
{
    // calibrate odom
    odom.calibrate(calibrateIMU);
    Pose newPose(x, y, theta);
    Chassis::setPose(newPose);
    // start the chassis task if it doesn't exist
    if (task == nullptr)
    {
        task = make_unique<vex::task>(odomLoop);
        printf("new chassis thead\n");
    }
    else
    {
        printf("thread already exists\n");
    }
}

/**
 * Set the pose of the chassis
 *
 * This function is a wrapper for the Odometry::setPose() function
 */
void Chassis::setPose(float x, float y, float theta, bool radians)
{
    Pose pose(x, y, theta);
    setPose(pose, radians);
}

/**
 * Set the pose of the chassis
 *
 * This function is a wrapper for the Odometry::setPose() function
 * but it also transforms the pose to the format needed by the user
 */
void Chassis::setPose(Pose pose, bool radians)
{
    if (!radians)
        pose.theta = degToRad(pose.theta);
    // pose.theta = M_PI_2 - pose.theta;
    odom.setPose(pose);
}

/**
 * Get the pose of the chassis
 *
 * This function is a wrapper for the Odometry::getPose() function
 * but it also transforms the pose to the format needed by the user
 */
Pose Chassis::getPose(bool radians)
{
    Pose pose = odom.getPose();
    // pose.theta = M_PI_2 - pose.theta;
    if (!radians)
        pose.theta = radToDeg(pose.theta);
    return pose;
}

/**
 * Wait until the robot has traveled a certain distance, or angle
 *
 * @note Units are in inches if current motion is moveTo or follow, degrees if using turnTo
 *
 * Just uses a while loop and exits when the distance traveled is greater than the specified distance
 * or if the motion has finished
 */
void Chassis::waitUntilDist(float dist)
{
    // give the movement time to start
    vex::wait(10, vex::msec);
    // wait until the robot has travelled a certain distance
    /*while (movement != nullptr && movement->getDist() < dist && movement->getDist() >= prevDist)
    {
        prevDist = movement->getDist(); // update previous distance
        vex::wait(10, vex::msec);
    }*/
    // set prevDist to 0
    prevDist = 0;
}

/**
 * Wait until the robot has completed the current movement
 */
void Chassis::waitUntilDone()
{
    // give the movement time to start
    vex::wait(10, vex::msec);
    // wait until the movement is done
    /*while (movement != nullptr && movement->getDist() >= prevDist)
    {
        prevDist = movement->getDist(); // update previous distance
        vex::wait(10, vex::msec);
    }*/
    // set prevDist to 0
    prevDist = 0;
}

void Chassis::waitUntilError(float &error, float margin)
{
    do
    {
        wait(10, vex::msec);
    } while (fabs(error) > margin);
}

/**
 * This function sets up the Turn controller
 *
 * There are some things that need to be done before instantiating the movement however.
 * It needs to set up a PID which the movement will use to turn the robot. We also need
 * to convert the x and y values given passed in to a Pose object. All that needs to be
 * done then is to pass the parameters to a new instance of Turn, and set the movement
 * pointer.
 */

void Chassis::turnSettings(float kp, float ki, float kd)
{
    //  set up the PID
    angularPID.setGains(0, 0, kp, ki, kd);
    angularPID.reset();
    angularPID.setIntegral(angularSettings.kIStart, angularSettings.kIMax);
    // setup the statemachine
    autoChassis = turnMode;
}

void Chassis::boomerangSettings(float akp, float aki, float akd, float lkp, float lki, float lkd)
{
    //  set up the PIDs
    angularPID.setGains(0, 0, akp, aki, akd);
    linearPID.setGains(0, 0, lkp, lki, lkd);
    angularPID.reset();
    linearPID.reset();
    angularPID.setIntegral(angularSettings.kIStart, angularSettings.kIMax);
    linearPID.setIntegral(linearSettings.kIStart, linearSettings.kIMax);
    // setup the statemachine
    autoChassis = turnMode;
}

void Chassis::turnToPose(float x, float y, bool reversed, float maxSpeed, float kp, float ki, float kd, float breakAngle)
{
    turnSettings(kp, ki, kd);
    Pose target(x, y, 0);
    Turn::params(target, reversed, maxSpeed, false, false);
    waitUntilError(Turn::breakOutError, breakAngle);
}

void Chassis::SwingOnLeftToPose(float x, float y, bool reversed, float maxSpeed, float kp, float ki, float kd, float breakAngle)
{
    turnSettings(kp, ki, kd);
    Pose target(x, y, 0);
    Turn::params(target, reversed, maxSpeed, true, false);
    drivetrain.leftMotors->stop(vex::hold);
    drivetrain.rightMotors->stop(vex::coast);
    waitUntilError(Turn::breakOutError, breakAngle);
    drivetrain.leftMotors->stop(vex::coast);
    drivetrain.rightMotors->stop(vex::coast);
}

void Chassis::SwingOnRightToPose(float x, float y, bool reversed, float maxSpeed, float kp, float ki, float kd, float breakAngle)
{
    turnSettings(kp, ki, kd);
    Pose target(x, y, 0);
    Turn::params(target, reversed, maxSpeed, false, true);
    drivetrain.leftMotors->stop(vex::coast);
    drivetrain.rightMotors->stop(vex::hold);
    waitUntilError(Turn::breakOutError, breakAngle);
    drivetrain.leftMotors->stop(vex::coast);
    drivetrain.rightMotors->stop(vex::coast);
}

/**
 * This function sets up the Turn controller
 *
 * There are some things that need to be done before instantiating the movement however.
 * It needs to set up a PID which the movement will use to turn the robot. We also need to
 * convert the heading passed by the user to radians and standard position. All that needs to be
 * done then is to pass the parameters to a new instance of Turn, and set the movement
 * pointer.
 */

void Chassis::turnToHeading(float heading, bool reversed, float maxSpeed, float kp, float ki, float kd, float breakAngle)
{
    turnSettings(kp, ki, kd);
    Turn::params(heading, reversed, maxSpeed, false, false);
    waitUntilError(Turn::breakOutError, breakAngle);
}

void Chassis::SwingOnLeftToHeading(float heading, bool reversed, float maxSpeed, float kp, float ki, float kd, float breakAngle)
{
    turnSettings(kp, ki, kd);
    Turn::params(heading, reversed, maxSpeed, true, false);
    drivetrain.leftMotors->stop(vex::hold);
    drivetrain.rightMotors->stop(vex::coast);
    waitUntilError(Turn::breakOutError, breakAngle);
    drivetrain.leftMotors->stop(vex::coast);
    drivetrain.rightMotors->stop(vex::coast);
}

void Chassis::SwingOnRightToHeading(float heading, bool reversed, float maxSpeed, float kp, float ki, float kd, float breakAngle)
{
    turnSettings(kp, ki, kd);
    Turn::params(heading, reversed, maxSpeed, false, true);
    drivetrain.leftMotors->stop(vex::coast);
    drivetrain.rightMotors->stop(vex::hold);
    waitUntilError(Turn::breakOutError, breakAngle);
    drivetrain.leftMotors->stop(vex::coast);
    drivetrain.rightMotors->stop(vex::coast);
}


/**
 * This function sets up the Boomerang controller
 *
 * Like all chassis movement functions, it sets a member pointer to a new movement.
 * the movement is a derived class of the Movement class.
 *
 * There are some things that need to be done before instantiating the movement however.
 * Two PIDs need to be set up to be passed to the Boomerang constructor, and the target heading
 * needs to be converted to radians and standard form.
 * It also needs to decide what the chasePower should be. Usually this will be the value set in
 * the drivetrain struct, but it can be overridden by the user if needed.
 */
void Chassis::moveTo(float x, float y, float theta, bool reversed, float lmaxSpeed, float amaxSpeed, float lkp, float lki, float lkd, float akp, float aki, float akd, float chasePower, float lead, float breakDist)
{
    Pose target = Pose(x, y, theta);
    boomerangSettings(akp, aki, akd, lkp, lki, lkd);
    MoveTo::params(target, reversed, lmaxSpeed, amaxSpeed, lead, chasePower);
    MoveTo::state = 0;
    waitUntilError(MoveTo::breakOutError, breakDist);
    autoChassis = moveToMode;
}

/**
 * This function sets up Pure Pursuit
 *
 * Unlike the chassis::moveTo function, we can just pass the parameters directly to the
 * Pure Pursuit constructor
 */
void Chassis::follow(float path, float lookahead, int timeout, bool forwards, int maxSpeed)
{
    // if a movement is already running, wait until it is done
    // if (movement != nullptr)
    //    waitUntilDone();
    // create the movement
    // movement = make_unique<PurePursuit>(drivetrain.trackWidth, path, lookahead, timeout, forwards, maxSpeed);
}

void Chassis::stateMachineOn()
{
    StateMachineEnabled = true;
}

void Chassis::stateMachineOff()
{
    StateMachineEnabled = false;
}

/**
 * Chassis statemachine function
 *
 * This function is called in a the chassis update task
 * It updates any motion controller that may be running
 */

std::pair<float, float> Chassis::stateMachine()
{
    switch (autoChassis)
    {
    case disabledMode:
        return {0, 0};
    case turnMode:
        return Turn::update(this->getPose());
    case moveToMode:
        return MoveTo::update(this->getPose());
    case followMode:
        return {0, 0};

    default:
        return {0, 0};
    }
}

/**
 * Chassis update function
 *
 * This function is called in a loop by the chassis task
 * It updates any motion controller that may be running
 * And it updates the odometry
 * Once implemented, it will also update the drivetrain velocity controllers
 */
void Chassis::update()
{
    // update odometry
    odom.update();

    if (StateMachineEnabled)
    {
        std::pair<float, float> output = stateMachine(); // get output
        // move the motors
        drivetrain.leftMotors->spin(vex::fwd, output.first, vex::volt);
        drivetrain.rightMotors->spin(vex::fwd, output.second, vex::volt);
    }
}
