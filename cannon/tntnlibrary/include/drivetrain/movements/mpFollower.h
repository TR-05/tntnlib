#pragma once
#include <utility>
#include "../tntnlibrary/include/drivetrain/movements/motionProfiling/profiler.h"

namespace tntnlib
{
    namespace MotionProfile
    {
        void params(float d, float vMax, float vi, float vf, float a);
        extern Profile profile;
        std::pair<float, float> update();
        extern float currentT;
    };
}; // namespace tntnlib
