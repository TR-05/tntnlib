#include "../tntnlibrary/include/drivetrain/movements/motionProfiling/profiler.h"
#include "../tntnlibrary/include/util.h"
#include <iostream>
#include <cstdio>
#include <cmath>
using namespace tntnlib;

void Profile::profileTime()
{
    //vf^2=vi^2+2ad
    float potentialMaxV = sqrt(fabs(this->a*this->d) );
    if (potentialMaxV < fabs(this->vMax))
    {
        vMax = potentialMaxV;
    }
    this->t1 = kinematic::accelerationTime(vMax * sgn(this->d) - vi, this->a);
    this->t3 = kinematic::accelerationTime(vf - vMax * sgn(this->d), this->a);
    this->d1 = kinematic::accelerationDistance(vi, vMax * sgn(this->d), this->a * sgn(this->d));
    this->d3 = kinematic::accelerationDistance(vMax * sgn(this->d), vf, -1 * this->a * sgn(this->d));
    this->d2 = this->d - d1 - d3;
    this->t2 = fabs(d2 / vMax);
    this->t = t1 + t2 + t3;
    printf("t1: %.2f, t2: %.2f, t3: %.2f, d1: %.2f, d2: %.2f, d3: %.2f, t: %.2f\n", t1, t2, t3, d1, d2, d3, t);
}
void Profile::generateProfile()
{
    this->profileTime();
    // t is in seconds and we want to have 10ms intervals
    // so we multiply by 1000.0 to convert to ms and then divide by 10 to get the number of 10 ms intervals
    this->V.resize(int(t * 100.0));
    this->D.resize(V.size());
    this->A.resize(V.size());
    float timeIterator = 0;
    for (int i = 0; i < V.size(); i++)
    {
        int phase = 0;
        if (timeIterator < this->t1)
            phase = 1;
        else if (timeIterator < this->t1 + this->t2)
            phase = 2;
        else
            phase = 3;

        switch (phase)
        {
        case 1:
            this->V[i] = this->vi + this->a * timeIterator * sgn(this->d);
            this->D[i] = this->vi * timeIterator + 0.5 * this->a * sgn(this->d) * timeIterator * timeIterator;
            this->A[i] = this->a * sgn(this->d);
            break;
        case 2:
            this->V[i] = this->vMax * sgn(this->d);
            this->D[i] = this->d1 + this->vMax * sgn(this->d) * (timeIterator - this->t1);
            this->A[i] = 0;
            break;
        case 3:
            this->V[i] = this->vMax * sgn(this->d) + this->a * sgn(this->d) * (this->t1 + this->t2 - timeIterator);
            this->D[i] = this->d1 + this->d2 + this->vMax * sgn(this->d) * (timeIterator - this->t1 - this->t2) + 0.5 * this->a * sgn(this->d) * (timeIterator - this->t1 - this->t2) * (timeIterator - this->t1 - this->t2);
            this->A[i] = -1 * this->a * sgn(this->d);
            break;
        }
        timeIterator += 10/1000.0;
    }
}
