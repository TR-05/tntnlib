#pragma once

namespace tntnlib
{
    namespace kinematic
    {
        float accelerationTime(float deltaV, float a);
        float accelerationDistance(float vi, float vf, float a);
    };
}; // namespace tntnlib
