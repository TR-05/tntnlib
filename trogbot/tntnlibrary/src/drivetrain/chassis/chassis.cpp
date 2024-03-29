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
#include "../tntnlibrary/include/drivetrain/movements/moveTo.h"
#include "../tntnlibrary/include/drivetrain/movements/turn.h"
#include "../tntnlibrary/include/drivetrain/movements/straightPid.h"
#include "../tntnlibrary/include/drivetrain/chassis/chassis.h"
#include "../tntnlibrary/include/drivetrain/chassis/odom.h"
#include <functional>

float akp = 0, aki = 0, akd = 0, lkp = 0, lki = 0, lkd = 0;
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


void Chassis::initialize(bool calibrateIMU, float x, float y, float theta)
{
    // calibrate odom
    odom.calibrate(calibrateIMU);
    Pose newPose(x, y, theta);
    Chassis::setPose(newPose);
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

Pose Chassis::getOffsetPose()
{
    return this->offsetPose;
}
void Chassis::setOffset(float x, float y)
{
    this->offsetPose.x = x;
    this->offsetPose.y = y;
}

void Chassis::startAuto(float x, float y, float theta)
{
    printf("Entered Auto\n");
    startTimer();
    chassis.initialize(false, x, y, theta);
    chassis.setOffset(0, 0);
    chassis.stateMachineOn();
    chassis.breakOutTimeMs = 15000;
    akp = chassis.angularSettings.kP;
    aki = chassis.angularSettings.kI;
    akd = chassis.angularSettings.kD;
    lkp = chassis.linearSettings.kP;
    lki = chassis.linearSettings.kI;
    lkd = chassis.linearSettings.kD;
}

void Chassis::stopAuto()
{
  chassis.stateMachineOff();
  delay(20);
  chassis.tank(0, 0, 1, 0, 100, 0);
  endTimer();
  printf("Time: %.2f\n", totalRunTime);
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
    float initialT = time(true);
    float Time = 0;
    do
    {
        Time = time(true) - initialT;
        if (Time > this->breakOutTimeMs)
            break;
        wait(10, vex::msec);
    } while (fabs(error) > margin && margin != 0);
}

void Chassis::LineWait(float x, float y, float margin, float maxTime)
{
    float initialT = Brain.timer(vex::msec) / 1000.0;
    float time = 0;
    Pose target(x, y, 0);
    do
    {
        time = Brain.timer(vex::msec) / 1000.0 - initialT;
        if (time > maxTime)
            break;
        wait(10, vex::msec);
    } while (target.distance(chassis.getPose(false)) > margin && margin != 0);
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
    motorControl = voltage;
    //  set up the PID
    angularPID.setGains(0, 0, kp, ki, kd);
    angularPID.reset();
    angularPID.setIntegral(angularSettings.kIStart, angularSettings.kIMax);
    // setup the statemachine
    autoChassis = turnMode;
}

void Chassis::moveToSettings(float akp, float aki, float akd, float lkp, float lki, float lkd, float slew)
{
    //  set up the PIDs
    motorControl = voltage;
    angularPID.setGains(0, 0, akp, aki, akd);
    linearPID.setGains(0, slew, lkp, lki, lkd);
    angularPID.reset();
    linearPID.reset();
    angularPID.setIntegral(angularSettings.kIStart, angularSettings.kIMax);
    linearPID.setIntegral(linearSettings.kIStart, linearSettings.kIMax);
}

void Chassis::turnToPose(float x, float y, bool reversed, float maxSpeed, float kp, float ki, float kd, float breakAngle)
{
    turnSettings(kp, ki, kd);
    Pose target(x, y, 0);
    Turn::params(target, reversed, maxSpeed, false, false, true);
    waitUntilError(Turn::breakOutError, breakAngle);
}

void Chassis::SwingOnLeftToPose(float x, float y, bool reversed, float maxSpeed, float kp, float ki, float kd, float breakAngle)
{
    turnSettings(kp, ki, kd);
    Pose target(x, y, 0);
    Turn::params(target, reversed, maxSpeed, true, false, true);
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
    Turn::params(target, reversed, maxSpeed, false, true, true);
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
    Turn::params(heading, reversed, maxSpeed, false, false, true);
    waitUntilError(Turn::breakOutError, breakAngle);
}

void Chassis::turnToHeadingUnbounded(float heading, bool reversed, float maxSpeed, float kp, float ki, float kd, float breakAngle)
{
    turnSettings(kp, ki, kd);
    Turn::params(heading, reversed, maxSpeed, false, false, false);
    waitUntilError(Turn::breakOutError, breakAngle);
}

void Chassis::tuneOffsets(float ang, float kp, float ki, float kd, float maxSpeed, float breakang)
{
    motorControl = voltage;
    Chassis::turnToHeadingUnbounded(ang, false, maxSpeed, kp, ki, kd, breakang);
    vex::wait(1000, vex::msec);
    stateMachineOff();
    chassis.tank(0, 0, 1,0,100,0);
    while (!Brain.Screen.pressing())
        vex::wait(10, vex::msec);
    vex::wait(500, vex::msec);
    float temporary_multiplier = ang / chassis.getPose().theta;
    temporary_multiplier *= sensors.gyro->getMultiplier();
    Brain.Screen.print("M:%f", temporary_multiplier);
    printf("\n copy this multiplier into data bot constructor in main: M:%f\n\n", temporary_multiplier);
    float ss = sensors.horizontal1->getDistance() / (2 * M_PI * ang / 360);
    float sr = sensors.vertical1->getDistance() / (2 * M_PI * ang / 360);
    Brain.Screen.setCursor(2, 0);
    Brain.Screen.print("\nSS:%f SR%f", ss, sr);
    printf("\nSS:%f SR%f\n\n", ss, sr);
}

void Chassis::SwingOnLeftToHeading(float heading, bool reversed, float maxSpeed, float kp, float ki, float kd, float breakAngle)
{
    turnSettings(kp, ki, kd);
    Turn::params(heading, reversed, maxSpeed, true, false, true);
    drivetrain.leftMotors->stop(vex::hold);
    drivetrain.rightMotors->stop(vex::coast);
    waitUntilError(Turn::breakOutError, breakAngle);
    drivetrain.leftMotors->stop(vex::coast);
    drivetrain.rightMotors->stop(vex::coast);
}

void Chassis::SwingOnRightToHeading(float heading, bool reversed, float maxSpeed, float kp, float ki, float kd, float breakAngle)
{
    turnSettings(kp, ki, kd);
    Turn::params(heading, reversed, maxSpeed, false, true, true);
    drivetrain.leftMotors->stop(vex::coast);
    drivetrain.rightMotors->stop(vex::hold);
    waitUntilError(Turn::breakOutError, breakAngle);
    drivetrain.leftMotors->stop(vex::coast);
    drivetrain.rightMotors->stop(vex::coast);
}

void Chassis::pid(float dist, float heading, bool reversed, float lmaxSpeed, float amaxSpeed, float lkp, float lki, float lkd, float akp, float aki, float akd, float slew, float breakDist)
{
    Pose target = Pose(0, 0, 0);
    straightPid::params(reversed, true, false, dist, lmaxSpeed, amaxSpeed, heading, target);
    moveToSettings(akp, aki, akd, lkp, lki, lkd, slew);
    // setup the statemachine
    autoChassis = drivePidMode;
    waitUntilError(straightPid::breakOutError, breakDist);
}

void Chassis::pid(float dist, float x, float y, bool reversed, float lmaxSpeed, float amaxSpeed, float lkp, float lki, float lkd, float akp, float aki, float akd, float slew, float breakDist)
{
    Pose target = Pose(x, y, 0);
    straightPid::params(reversed, true, true, dist, lmaxSpeed, amaxSpeed, 0, target);
    moveToSettings(akp, aki, akd, lkp, lki, lkd, slew);
    // setup the statemachine
    autoChassis = drivePidMode;
    waitUntilError(straightPid::breakOutError, breakDist);
}

void Chassis::arcPid(float arclength, float radius, int direction, float lmaxSpeed, float lkp, float lki, float lkd, float akp, float aki, float akd, float slew, float breakDist)
{
    tntnlib::arcPid::params(arclength, radius, direction, lmaxSpeed, breakDist);
    moveToSettings(akp, aki, akd, lkp, lki, lkd, slew);

    motorControl = voltage;
    // setup the statemachine
    autoChassis = arcPidMode;
    waitUntilError(arcPid::breakOutError, breakDist);
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
void Chassis::boomerangTo(float x, float y, float theta, bool reversed, float lmaxSpeed, float amaxSpeed, float lkp, float lki, float lkd, float akp, float aki, float akd, float chasePower, float lead, float slew, float breakDist)
{
    Pose target = Pose(x, y, theta);
    MoveTo::targetChoice = MoveTo::boomerangTargetMode;
    MoveTo::params(target, reversed, lmaxSpeed, amaxSpeed, lead, chasePower);
    moveToSettings(akp, aki, akd, lkp, lki, lkd, slew);
    // setup the statemachine
    autoChassis = moveToMode;
    waitUntilError(MoveTo::breakOutError, breakDist);
}

void Chassis::moveTo(float x, float y, bool reversed, float lmaxSpeed, float amaxSpeed, float lkp, float lki, float lkd, float akp, float aki, float akd, float slew, float breakDist)
{
    Pose target = Pose(x, y, 0);
    MoveTo::targetChoice = MoveTo::staticTargetMode;
    MoveTo::params(target, reversed, lmaxSpeed, amaxSpeed);
    moveToSettings(akp, aki, akd, lkp, lki, lkd, slew);
    // setup the statemachine
    autoChassis = moveToMode;
    waitUntilError(MoveTo::breakOutError, breakDist);
}

/**
 * This function sets up Pure Pursuit
 *
 */
void Chassis::follow(Path &path, bool reversed, float lmaxSpeed, float amaxSpeed, float lkp, float lki, float lkd, float akp, float aki, float akd, float slew, float lookAhead, float breakDist)
{
    Pose target = Pose(path.x3, path.y3, path.theta[path.numberOfPoints]);
    MoveTo::targetChoice = MoveTo::purePursuitTargetMode;
    MoveTo::params(target, path, reversed, lmaxSpeed, amaxSpeed, lookAhead, 0);
    moveToSettings(akp, aki, akd, lkp, lki, lkd, slew);
    // setup the statemachine
    autoChassis = moveToMode;
    waitUntilError(MoveTo::breakOutError, breakDist);
}

void Chassis::autoTankVolts(float left, float right)
{
    this->leftVolts = left, this->rightVolts = right;
    // setup the statemachine
    this->motorControl = voltage;
    this->autoChassis = voltageMode;
}

void Chassis::autoTankPct(float left, float right)
{
    leftVolts = left, rightVolts = right;
    // setup the statemachine
    motorControl = velocity;
    autoChassis = voltageMode;
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
        return MoveTo::update(this->getPose(), this->getOffsetPose());
    case followMode:
        return {0, 0};
    case drivePidMode:
        return straightPid::update(this->getPose());
    case arcPidMode:
        return arcPid::update(this->getPose());
    case voltageMode:
        return {leftVolts, rightVolts};
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
        if (motorControl == voltage)
        {
            drivetrain.leftMotors->spinVolts(output.first);
            drivetrain.rightMotors->spinVolts(output.second);
        }
        else if (motorControl == velocity)
        {
            drivetrain.leftMotors->spinPct(output.first);
            drivetrain.rightMotors->spinPct(output.second);
        }
    }
}
