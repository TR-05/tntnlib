#include <cmath>
#include "vex.h"
#include "../tntnlibrary/include/util.h"
#include "../tntnlibrary/include/drivetrain/pose.h"
#include "../tntnlibrary/include/drivetrain/movements/motionProfiling/profiler.h"   
#include "../tntnlibrary/include/drivetrain/movements/mpFollower.h"   
#include "vex.h"

using namespace tntnlib;

Profile MotionProfile::profile(0, 0, 0, 0, 0);
float MotionProfile::currentT = 0;
void MotionProfile::params(float d, float vMax, float vi, float vf, float a)
{
    currentT = 0;
    MotionProfile::profile = Profile(d, vMax, vi, vf, a);
}
std::pair<float, float> MotionProfile::update()
{
    float leftPower = MotionProfile::profile.V[currentT];
    float rightPower = MotionProfile::profile.V[currentT];
    currentT++;
    return {leftPower, rightPower};
    // return {0, 0};
}
