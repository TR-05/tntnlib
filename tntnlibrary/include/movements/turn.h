#pragma once

#include <utility>
#include "../tntnlibrary/include/pose.h"
#include "../tntnlibrary/include/pid.h"

namespace tntnlib
{
        /**
         * @brief Turn namespace
         */
        namespace Turn
        {
                /**
                 * @brief Construct a new Turn movement
                 *
                 * @param angularPID the angular PID to use
                 * @param target the target heading. Radians, 0 is right, increases counterclockwise
                 * @param maxSpeed the maximum speed the robot can turn at
                 */
                void params(float target, bool reversed, float maxSpeed, bool swingOnLeft, bool swingOnRight, bool boundto360);
                // Turn(FAPID angularPID, float target, int maxSpeed);

                /**
                 * @brief Construct a new Turn movement
                 *
                 * @param angularPID the angular PID to use
                 * @param target the target pose
                 * @param reversed whether the robot should face the point with its back or front
                 * @param maxSpeed the maximum speed the robot can turn at
                 */
                void params(Pose target, bool reversed, float maxSpeed, bool swingOnLeft, bool swingOnRight, bool boundto360);
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

                struct turnSettings_t
                {
                        Pose startPose = Pose(0, 0, 0);
                        Pose targetPose = Pose(0, 0, 0);
                        float targetHeading = 0;
                        bool reversed = false;
                        float maxSpeed;
                        int state = 0; // 0 = in progress, 1 = done
                        float dist = 0;
                        bool useHeading = true;
                        bool swingOnRight = false;
                        bool swingOnLeft = false;
                        bool boundto360 = false;
                } static turnSettings;
                inline float breakOutError = 0;
        };
}; // namespace tntnlib
