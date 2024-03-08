#include "../tntnlibrary/include/drivetrain/movements/motionProfiling/kinematics.h"
#include <cmath>
using namespace tntnlib;

float kinematic::accelerationTime(float deltaV, float a)
{
    return fabs(deltaV / a);
}

float kinematic::accelerationDistance(float vi, float vf, float a)
{
    float t = accelerationTime(vf - vi, a);
    return (vi*t + 0.5*a*t*t);
}
