#pragma once
#include <vector>
#include "../tntnlibrary/include/drivetrain/movements/motionProfiling/kinematics.h"
namespace tntnlib
{
    class Profile
    {
        public:
        Profile(float d, float vMax, float vi, float vf, float a)
        : d(d), vMax(vMax), vi(vi), vf(vf), a(a)
        {
            generateProfile();
        }

        std::vector<float> V;
        std::vector<float> D;
        std::vector<float> A;

        void generateProfile();
        void profileTime();
        float d, t, vMax, vi, vf, a, t1, t2, t3, d1, d2, d3;
    };
}; // namespace tntnlib
