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
// #include "tntnlib/movements/boomerang.h"
#include "tntnlib/movements/purepursuit.h"
#include "tntnlib/movements/turn.h"
#include "tntnlib/chassis/chassis.h"
#include "tntnlib/chassis/odom.h"

using namespace tntnlib;

// make_unique c++11 implementation
template <class T, class... Args>
std::unique_ptr<T> make_unique(Args &&...args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

/**
 * Initialize the chassis
 *
 * Calibrates sensors and starts the chassis task
 */

void Chassis::initialize(bool calibrateIMU)
{
    // calibrate odom
    odom.calibrate(calibrateIMU);
    // start the chassis task if it doesn't exist
  /*  if (task == nullptr) 
            task = make_unique<vex::task>([&]() {
            while (true) {
                update();
                vex::wait(10, vex::msec);
            }
        });*/
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
    pose.theta = M_PI_2 - pose.theta;
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
    pose.theta = M_PI_2 - pose.theta;
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
    while (movement != nullptr && movement->getDist() < dist && movement->getDist() >= prevDist)
    {
        prevDist = movement->getDist(); // update previous distance
        vex::wait(10, vex::msec);
    }
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
    while (movement != nullptr && movement->getDist() >= prevDist)
    {
        prevDist = movement->getDist(); // update previous distance
        vex::wait(10, vex::msec);
    }
    // set prevDist to 0
    prevDist = 0;
}

/**
 * This function sets up the Turn controller
 *
 * Like all chassis movement functions, it sets a member pointer to a new movement.
 * the movement is a derived class of the Movement class
 *
 * There are some things that need to be done before instantiating the movement however.
 * It needs to set up a PID which the movement will use to turn the robot. We also need
 * to convert the x and y values given passed in to a Pose object. All that needs to be
 * done then is to pass the parameters to a new instance of Turn, and set the movement
 * pointer.
 */
void Chassis::turnToPose(float x, float y, int timeout, bool reversed, int maxSpeed)
{
    // if a movement is already running, wait until it is done
    if (movement != nullptr)
        waitUntilDone();
    // set up the PID
    FAPID angularPID(0, 0, angularSettings.kP, 0, angularSettings.kD);
    angularPID.setExit(angularSettings.largeError, angularSettings.smallError, angularSettings.largeErrorTimeout,
                       angularSettings.smallErrorTimeout, timeout);
    // create the movement
    movement = make_unique<Turn>(angularPID, Pose(x, y), reversed, maxSpeed);
}

/**
 * This function sets up the Turn controller
 *
 * Like all chassis movement functions, it sets a member pointer to a new movement.
 * the movement is a derived class of the Movement class
 *
 * There are some things that need to be done before instantiating the movement however.
 * It needs to set up a PID which the movement will use to turn the robot. We also need to
 * convert the heading passed by the user to radians and standard position. All that needs to be
 * done then is to pass the parameters to a new instance of Turn, and set the movement
 * pointer.
 */
void Chassis::turnToHeading(float heading, int timeout, int maxSpeed)
{
    // if a movement is already running, wait until it is done
    if (movement != nullptr)
        waitUntilDone();
    // convert heading to radians and standard form
    float newHeading = M_PI_2 - degToRad(heading);
    // set up the PID
    FAPID angularPID(0, 0, angularSettings.kP, 0, angularSettings.kD);
    angularPID.setExit(angularSettings.largeError, angularSettings.smallError, angularSettings.largeErrorTimeout,
                       angularSettings.smallErrorTimeout, timeout);
    // create the movement
    movement = make_unique<Turn>(angularPID, newHeading, maxSpeed);
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
void Chassis::moveTo(float x, float y, float theta, int timeout, bool forwards, float chasePower, float lead,
                     int maxSpeed)
{
    // if a movement is already running, wait until it is done
    if (movement != nullptr)
        waitUntilDone();
    // convert target theta to radians and standard form
    Pose target = Pose(x, y, M_PI_2 - degToRad(theta));
    // set up PIDs
    FAPID linearPID(0, 0, lateralSettings.kP, 0, lateralSettings.kD);
    linearPID.setExit(lateralSettings.largeError, lateralSettings.smallError, lateralSettings.largeErrorTimeout,
                      lateralSettings.smallErrorTimeout, timeout);
    FAPID angularPID(0, 0, angularSettings.kP, 0, angularSettings.kD);
    // if chasePower is 0, is the value defined in the drivetrain struct
    if (chasePower == 0)
        chasePower = drivetrain.chasePower;
    // create the movement
    // movement = make_unique<Boomerang>(linearPID, angularPID, target, forwards, chasePower, lead, maxSpeed);
}

/**
 * Move the robot with a custom motion algorithm
 */
void Chassis::moveCustom(std::unique_ptr<Movement> movement)
{
    // if a movement is already running, wait until it is done
    if (movement != nullptr)
        waitUntilDone();
    // create the movement
    this->movement = std::move(movement);
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
    if (movement != nullptr)
        waitUntilDone();
    // create the movement
    // movement = make_unique<PurePursuit>(drivetrain.trackWidth, path, lookahead, timeout, forwards, maxSpeed);
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
    // update the motion controller, if one is running
    if (movement != nullptr)
    {
        std::pair<int, int> output = movement->update(odom.getPose()); // get output
        if (output.first == 128 && output.second == 128)
        {                       // if the movement is done
            movement = nullptr; // stop movement
            output.first = 0;
            output.second = 0;
        }
        // move the motors
        drivetrain.leftMotors->spin(vex::fwd, output.first, vex::volt);
        drivetrain.rightMotors->spin(vex::fwd, output.second, vex::volt);
    }
}