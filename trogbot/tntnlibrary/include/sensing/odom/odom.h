#pragma once

#include "../tntnlibrary/include/sensing/odom/pose.h"
#include "../tntnlibrary/include/sensing/sensors/gyro.h"
#include "../tntnlibrary/include/sensing/sensors/trackingWheel.h"
#include "vex.h"

namespace tntnlib
{
class Odometry
{
   public:
    Odometry(TrackingWheel *vertical1, TrackingWheel *vertical2, TrackingWheel *horizontal1,
             TrackingWheel *horizontal2, Gyro *imu) : vertical1(vertical1),
                                                      vertical2(vertical2),
                                                      horizontal1(horizontal1),
                                                      horizontal2(horizontal2),
                                                      gyro(imu) {}
    /**
     * @brief Calibrate sensors used for odometry
     *
     * @param calibrateIMU whether to calibrate the imu.
     */
    void calibrate(bool calibrateIMU);

    /**
     * @brief Get the Pose
     *
     * @note Units are in radians. Locked from 0 to 2pi. Right is 0, increasing counter-clockwise
     *
     * @return Pose
     */
    Pose getPose();

    /**
     * @brief Set the Pose
     *
     * @note Units are in radians. Right is 0, increasing counter-clockwise
     *
     * @param pose what the pose should be set to
     */
    void setPose(Pose pose);

    /**
     * @brief Update the pose of the robot
     *
     * @note this should be called in a loop, ideally every 10ms (the sensor polling rate)
     */
    void update();

    TrackingWheel *vertical1;
    TrackingWheel *vertical2;
    TrackingWheel *horizontal1;
    TrackingWheel *horizontal2;
    Gyro *gyro;

   private:
    Pose pose = Pose(0, 0, 0);
    float prevVertical = 0;
    float prevVertical1 = 0;
    float prevVertical2 = 0;
    float prevHorizontal = 0;
    float prevHorizontal1 = 0;
    float prevHorizontal2 = 0;
    float prevImu = 0;
};
}  // namespace tntnlib
