#pragma once
#include "vex.h"
#include "../tntnlibrary/include/pathing/cubicBezier.h"
#include "../tntnlibrary/include/pose.h"

#include <iostream>
namespace tntnlib
{
    class PurePursuit
    {
        friend class Pose;

    private:
        float min(float first, float second);
        float max(float first, float second);
        float hypotenuse(float x1, float y1, float x2, float y2);

    public:
        Pose tempTarget = Pose(0, 0, 0);
        Pose target = Pose(0, 0, 0);

        int last_found_index = 1, starting_index = 0;
        void searchForIntersect(Path &path, Pose &pose, float lookAhead);
        void getNextIntersect(Path &path, Pose &pose, float lookAhead);
    };

} // namespace tntnlib
extern tntnlib::PurePursuit purePursuit;
