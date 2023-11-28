#pragma once

#include <utility>
#include "../tntnlibrary/include/pose.h"
#include "../tntnlibrary/include/pathing/cubicBezier.h"
#include "../tntnlibrary/include/pid.h"

namespace tntnlib
{
    /**
     * @brief Turn namespace
     */
    namespace MoveTo
    {

        /**
         * @brief Construct a new Turn movement
         *
         * @param angularPID the angular PID to use
         * @param target the target pose
         * @param reversed whether the robot should face the point with its back or front
         * @param maxSpeed the maximum speed the robot can turn at
         */
        void params(Pose target, bool reversed, float lmaxSpeed, float amaxSpeed);
        void params(Pose target, bool reversed, float lmaxSpeed, float amaxSpeed, float lead, float chasePower);
        void params(Pose target, Path path, bool reversed, float lmaxSpeed, float amaxSpeed, float lookAhead, float chasePower);

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


        /**
         * @brief Get the distance travelled during the movement
         *
         * @return float
         */
        void updateTarget(Pose &pose);

        enum targetMode
        {
            staticTargetMode,
            boomerangTargetMode,
            purePursuitTargetMode
        };
        inline targetMode targetChoice = staticTargetMode;

        inline Pose targetPose = Pose(0, 0, 0);
        inline Pose currentTargetPose = Pose(0, 0, 0);
        inline Pose carrot = Pose(0, 0, 0);

        inline float targetHeading = 0;
        inline bool reversed = false;
        inline float lmaxSpeed;
        inline float amaxSpeed;
        inline int state = 0; // 0 = in progress, 1 = done
        inline float lead = 0;
        inline float chasePower = 0;
        inline float lookAhead = 0;
        inline float breakOutError = 0;
        extern tntnlib::Path path;

    };
}; // namespace tntnlib
