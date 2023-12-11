#pragma once

#include <utility>
#include "../tntnlibrary/include/drivetrain/pose.h"
#include "../tntnlibrary/include/pid.h"

namespace tntnlib
{
    /**
     * @brief Turn namespace
     */
    namespace straightPid
    {

        /**
         * @brief Construct a new Turn movement
         *
         * @param angularPID the angular PID to use
         * @param target the target pose
         * @param reversed whether the robot should face the point with its back or front
         * @param maxSpeed the maximum speed the robot can turn at
         */
        void params(bool reversed, bool holdHeading, bool turnToPoint, float straightDistance, float lmaxSpeed, float amaxSpeed, float targetHeading, Pose targetPose);
        // Turn(FAPID angularPID, Pose target, bool reversed, int maxSpeed);
        /**
         * @brief Update the movement
         *
         * @note This function should be called in a loop
         * @note Units are in inches and radians. Angle 0 is right, and increases counterclockwise
         *
         * @param pose the position of the chassis
         *
         * @return std::pair<int, int> left and right motor power respectively. 128 means movement is done
         */
        std::pair<float, float> update(Pose pose);

        inline float straightDistance = 0;
        inline float startingVertical1Dist = 0;
        inline float targetHeading = 0;
        inline bool reversed = false;
        inline bool holdHeading = false;
        inline bool turnToPoint = false;        
        inline float lmaxSpeed = 0;
        inline float amaxSpeed = 0;
        inline float breakOutError = 0;
        inline Pose targetPose = Pose(0, 0, 0);
    };
}; // namespace tntnlib
