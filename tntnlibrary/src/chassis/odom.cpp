/**
 * @file src/tntnlib/odom.cpp
 * @author Trevor Ruttan
 * @brief Pose class declarations
 * @version 0.0
 * @date 2023-10-12
 *
 * @copyright Copyright (c) 2023
 *
 */

// The implementation below is mostly based off of
// the document written by 5225A (Pilons)
// Here is a link to the original document
// http://thepilons.ca/wp-content/uploads/2018/10/Tracking.pdf

#include <math.h>
#include <cerrno>
#include "vex.h"
#include "../tntnlibrary/include/util.h"
#include "../tntnlibrary/include/chassis/odom.h"
#include "../tntnlibrary/include/devices/trackingWheel.h"
#include "../tntnlibrary/include/devices/defaultDevices.h"

using namespace tntnlib;

/**
 * Calibrate odometry OdomSensors
 *
 * This function should be called before Odometry::update() runs for the first time.
 *
 * First, we attempt to calibrate the imu. IMU calibration failure is common, so we
 * attempt to calibrate it up to 5 times if it fails. If it still fails after this, we
 * rumble the controller to let the user know that the imu failed to calibrate. If it
 * does fail to calibrate, we will use other OdomSensors to calculate the heading of the robot.
 *
 * Calibrating the tracking wheels is simpler. If we detect there are less than 2 vertical
 * tracking wheels, we will simply substitute one side of the drivetrain, and then we
 * calibrate them. For horizontal wheels, we just calibrate them if they exist.
 */
void Odometry::calibrate(bool calibrateIMU)
{
    // calibrate the imu if it exists
    if (OdomSensors.imu != nullptr && calibrateIMU)
    {
        int attempt = 1;
        OdomSensors.imu->calibrate();
        // calibrate inertial, and if calibration fails, then repeat 5 times or until successful
        while (OdomSensors.imu->isCalibrating() != 1 && attempt < 5)
        {
            OdomSensors.imu->calibrate();
            attempt++;
            vex::wait(50, vex::msec);
        }
        if (attempt == 5)
        {
            // disable IMU
            OdomSensors.imu = nullptr;
            // rumble controller if imu calibration fails
            Controller.rumble("---");
            printf("IMU FAILED\n\n");
        } 
        else 
        {
            printf("IMU success\n\n");
        }
    }
    /*
    // substitute tracking wheels with a side of the drivetrain if needed (I dont like this)
    if (OdomSensors.vertical1 == nullptr)
        OdomSensors.vertical1 =
            new TrackingWheel(drive.leftMotors, drive.wheelDiameter, -(drive.trackWidth / 2), drive.cartridgeRPM, drive.rpm);
    if (OdomSensors.vertical2 == nullptr)
       OdomSensors.vertical2 = new TrackingWheel(drive.rightMotors, drive.wheelDiameter, drive.trackWidth / 2, drive.cartridgeRPM, drive.rpm);
    */

    // calibrate the tracking wheels
    if (OdomSensors.vertical1 != nullptr)
        OdomSensors.vertical1->reset();
    if (OdomSensors.vertical2 != nullptr)
        OdomSensors.vertical2->reset();

    if (OdomSensors.horizontal1 != nullptr)
        OdomSensors.horizontal1->reset();
    if (OdomSensors.horizontal2 != nullptr)
        OdomSensors.horizontal2->reset();
}

/**
 * Gets the pose
 */
Pose Odometry::getPose() { return pose; }

/**
 * Sets the pose
 */
void Odometry::setPose(Pose pose) { this->pose = pose; }

/**
 * Odometry update function
 *
 * Odometry is what we call position tracking. We use odometry so we can
 * use the position of the robot to correct for errors during autonomous.
 *
 * This function checks which OdomSensors are configured and uses them to calculate
 * the position of the robot. This ability means that almost any odometry configuration
 * will work with this library, all the user has to do is pass the OdomSensors.
 * It will also use the best combination of OdomSensors available, so if the user can
 * just pass everything and know the best OdomSensors will be used.
 *
 * In future, an extended Kalman filter will be used so all the OdomSensors can be used.
 */
void Odometry::update()
{
    // get the current sensor values
    float vertical1Raw = 0;
    float vertical2Raw = 0;
    float horizontal1Raw = 0;
    float horizontal2Raw = 0;
    float imuRaw = 0;
    if (OdomSensors.vertical1 != nullptr)
        vertical1Raw = OdomSensors.vertical1->getDistance();
    if (OdomSensors.vertical2 != nullptr)
        vertical2Raw = OdomSensors.vertical2->getDistance();
    if (OdomSensors.horizontal1 != nullptr)
        horizontal1Raw = OdomSensors.horizontal1->getDistance();
    if (OdomSensors.horizontal2 != nullptr)
        horizontal2Raw = OdomSensors.horizontal2->getDistance();
    if (OdomSensors.imu != nullptr)
        imuRaw = degToRad(OdomSensors.imu->rotation());

    // calculate the change in sensor values
    float deltaVertical1 = vertical1Raw - prevVertical1;
    float deltaVertical2 = vertical2Raw - prevVertical2;
    float deltaHorizontal1 = horizontal1Raw - prevHorizontal1;
    float deltaHorizontal2 = horizontal2Raw - prevHorizontal2;
    float deltaImu = imuRaw - prevImu;

    // update the previous sensor values
    prevVertical1 = vertical1Raw;
    prevVertical2 = vertical2Raw;
    prevHorizontal1 = horizontal1Raw;
    prevHorizontal2 = horizontal2Raw;
    prevImu = imuRaw;

    // calculate the heading of the robot
    // Priority:
    // 1. IMU
    // 2. Horizontal tracking wheels
    // 3. Vertical tracking wheels

    float heading = pose.theta;
    // calculate heading with inertial sensor if it exists
    if (OdomSensors.imu != nullptr)
        heading += deltaImu;
    // else, use horizontal tracking wheels if they both exist
    else if (OdomSensors.horizontal1 != nullptr && OdomSensors.horizontal2 != nullptr)
        heading += (deltaHorizontal1 - deltaHorizontal2) /
                   (OdomSensors.horizontal1->getOffset() - OdomSensors.horizontal2->getOffset());
    else
        heading +=
            (deltaVertical1 - deltaVertical2) / (OdomSensors.vertical1->getOffset() - OdomSensors.vertical2->getOffset());
    float deltaHeading = heading - pose.theta;
    float avgHeading = pose.theta + deltaHeading / 2;

    // choose tracking wheels to use
    TrackingWheel *verticalWheel = OdomSensors.vertical1;
    TrackingWheel *horizontalWheel = nullptr;
    // horizontal tracking wheels
    if (OdomSensors.horizontal1 != nullptr)
        horizontalWheel = OdomSensors.horizontal1;
    if (OdomSensors.horizontal2 != nullptr)
        horizontalWheel = OdomSensors.horizontal2;

    // get raw values
    float rawVertical = 0;
    float rawHorizontal = 0;
    rawVertical = verticalWheel->getDistance();
    if (horizontalWheel != nullptr)
        rawHorizontal = horizontalWheel->getDistance();
    // get offsets
    float horizontalOffset = 0;
    float verticalOffset = 0;
    verticalOffset = verticalWheel->getOffset();
    if (horizontalWheel != nullptr)
        horizontalOffset = horizontalWheel->getOffset();

    // calculate change in x and y
    float deltaX = 0;
    float deltaY = 0;
    deltaY = rawVertical - prevVertical;
    if (horizontalWheel != nullptr)
        deltaX = rawHorizontal - prevHorizontal;
    prevVertical = rawVertical;
    prevHorizontal = rawHorizontal;

    // calculate local x and y
    float localX = 0;
    float localY = 0;
    if (deltaHeading == 0)
    { // prevent divide by 0
        localX = deltaX;
        localY = deltaY;
    }
    else
    {
        localX = 2 * sin(deltaHeading / 2) * (deltaX / deltaHeading + horizontalOffset);
        localY = 2 * sin(deltaHeading / 2) * (deltaY / deltaHeading + verticalOffset);
    }

    // calculate global x and y
    pose.x += localY * sin(avgHeading);
    pose.y += localY * cos(avgHeading);
    pose.x += localX * cos(avgHeading);
    pose.y += localX * -sin(avgHeading);
    pose.theta = heading;


    //Liam V2
    /*
    // calculate heading
float theta = pose.theta;
if (gyros.size() > 0) {
    std::vector<float> angles;
    for (const auto& gyro : gyros) angles.push_back(gyro->getRotationDelta());
    theta += avg(angles);
} else if (horizontals.size() > 1) {
    theta += (horizontals.at(0).getDistanceDelta() - horizontals.at(1).getDistanceDelta()) /
             (horizontals.at(0).getOffset() - horizontals.at(1).getOffset());
} else if (verticals.size() > 1) {
    theta += (verticals.at(0).getDistanceDelta() - verticals.at(1).getDistanceDelta()) /
             (verticals.at(0).getOffset() - verticals.at(1).getOffset());
} else {
    infoSink()->error("Odom calculation failure! Not enough OdomSensors to calculate heading");
    return;
}
const float deltaTheta = theta - pose.theta;
const float avgTheta = pose.theta + deltaTheta / 2;

// calculate local change in position
Pose local(0, 0, deltaTheta);
const float sinDTheta2 = (deltaTheta == 0) ? 1 : 2 * std::sin(deltaTheta / 2);
for (auto& tracker : horizontals) {
    const float radius = (deltaTheta == 0) ? tracker.getDistanceDelta()
                                           : tracker.getDistanceDelta() / deltaTheta + tracker.getOffset();
    local.x += sinDTheta2 * radius / horizontals.size();
}
for (auto& tracker : verticals) {
    const float radius = (deltaTheta == 0) ? tracker.getDistanceDelta()
                                           : tracker.getDistanceDelta() / deltaTheta + tracker.getOffset();
    local.y += sinDTheta2 * radius / horizontals.size();
}
if (verticals.empty()) infoSink()->warn("No vertical tracking wheels! Assuming movement is 0");

// calculate new position
pose += local.rotate(avgTheta);
    
    */
}
