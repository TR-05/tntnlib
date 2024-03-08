#pragma once

#include <utility>
#include "../tntnlibrary/include/drivetrain/pose.h"
#include "../tntnlibrary/include/pid.h"
#include "../tntnlibrary/include/drivetrain/movements/motionProfiling/profiler.h"
namespace tntnlib
{
    /**
     * @brief Turn namespace
     */
    namespace arcPid
    {
        void params(float arclength, float radius, int direction, float lmaxSpeed, float breakDist);
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

        extern float arclength, startingVertical1Dist, startTimeMs, startTheta, endTheta;
        extern float radius, lmaxSpeed, breakOutError;
        extern bool reversed;
        extern int direction; // must be 1 or -1
        extern tntnlib::Profile linearProfile;
    };
}; // namespace tntnlib
