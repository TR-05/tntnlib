#include "vex.h"
#include <vector>
#include <iostream>
#include "../tntnlibrary/include/pathing/cubicBezier.h"
#include "../tntnlibrary/include/devices/defaultDevices.h"

using namespace tntnlib;

Path Path::operator+(const Path &other)
{
    int newNumberOfPoints = this->numberOfPoints + other.numberOfPoints + 1;
    int oldNumberOfPoints = this->numberOfPoints  + 1;
    Path newPath(0, 0, 0, 0, 0, 0, 0, 0, newNumberOfPoints);
    newNumberOfPoints -= 1;
    for (int i = 0; i < newNumberOfPoints; i++)
    {
        if (i <= this->numberOfPoints)
        {
            newPath.x[i] = this->x[i];
            newPath.y[i] = this->y[i];
            newPath.theta[i] = this->theta[i];
            newPath.deltaTheta[i] = this->deltaTheta[i];
            newPath.t[i] = this->t[i];
            newPath.segmentLength[i] = this->segmentLength[i];
            newPath.segmentCurvature[i] = this->segmentCurvature[i];
        }
        else
        {
            newPath.x[i] = other.x[i - oldNumberOfPoints];
            newPath.y[i] = other.y[i - oldNumberOfPoints];
            newPath.theta[i] = other.theta[i - oldNumberOfPoints];
            newPath.deltaTheta[i] = other.deltaTheta[i - oldNumberOfPoints];
            newPath.t[i] = other.t[i - oldNumberOfPoints];
            newPath.segmentLength[i] = other.segmentLength[i - oldNumberOfPoints];
            newPath.segmentCurvature[i] = other.segmentCurvature[i - oldNumberOfPoints];
        }
        newPath.x[newNumberOfPoints] = other.x[other.numberOfPoints];
        newPath.y[newNumberOfPoints] = other.y[other.numberOfPoints];
        newPath.theta[newNumberOfPoints] = other.theta[other.numberOfPoints];
        newPath.deltaTheta[newNumberOfPoints] = other.deltaTheta[other.numberOfPoints];
        newPath.t[newNumberOfPoints] = other.t[other.numberOfPoints];
        newPath.segmentLength[newNumberOfPoints] = other.segmentLength[other.numberOfPoints];
        newPath.segmentCurvature[newNumberOfPoints] = other.segmentCurvature[other.numberOfPoints];
        newPath.x3 = other.x3;
        newPath.y3 = other.y3;
    }

    return newPath;
}

float Path::cubicInterpolate(float p0, float p1, float p2, float p3, float t)
{ // interpolates between the 4 given points
    float pointT = (pow((1 - t), 3) * p0) + (3 * pow((1 - t), 2) * t * p1) + (3 * (1 - t) * t * t * p2) + (t * t * t * p3);
    return pointT;
}

float Path::cubicInterpolateD1(float p0, float p1, float p2, float p3, float t)
{ // interpolates between the 4 given points
    float C1 = (p3 - (3.0 * p2) + (3.0 * p1) - p0);
    float C2 = ((3.0 * p2) - (6.0 * p1) + (3.0 * p0));
    float C3 = ((3.0 * p1) - (3.0 * p0));
    // finally it is easy to calculate the slope element,
    // using those coefficients:
    return ((3.0 * C1 * t * t) + (2.0 * C2 * t) + C3);
}

float Path::cubicInterpolateDerivativeDegrees(float t)
{
    float rise = cubicInterpolateD1(y0, y1, y2, y3, t);
    float run = cubicInterpolateD1(x0, x1, x2, x3, t);
    float thetaRad = atan2(run, rise);
    float thetaDegrees = (thetaRad * (180 / M_PI));
    return thetaDegrees;
}

float Path::cubicInterpolateD2(float p0, float p1, float p2, float p3, float t)
{ // interpolates between the 4 given points
    float C1 = 6 * (1 - t) * p0;
    float C2 = (-12 + 18 * t) * p1;
    float C3 = ((6 - 18 * t) * p2);
    float C4 = (6 * t * p3);
    // finally it is easy to calculate the slope element,
    // using those coefficients:
    return C1 + C2 + C3 + C4;
}

void Path::generatePath()
{
    float t = 0;
    //int startTime = Brain.timer(vex::msec);
    //printf("\nstartTime: %d", startTime);
    for (int i = 0; i <= this->numberOfPoints; i++)
    {
        t = (float)i / numberOfPoints;
        if (t > 1)
            t = 1;
        this->t[i] = t;
        this->x[i] = cubicInterpolate(x0, x1, x2, x3, t);
        this->y[i] = cubicInterpolate(y0, y1, y2, y3, t);
        this->theta[i] = cubicInterpolateDerivativeDegrees(t);
        if (i != 0)
        {
            this->deltaTheta[i] = this->theta[i] - this->theta[i - 1];
            this->length += sqrt(pow(this->x[i] - this->x[i - 1], 2) + pow(this->y[i] - this->y[i - 1], 2));
        }
        else
            this->deltaTheta[0] = 0;
        /*path1.d1slope[i] = tangent_at(t, x0, y0, x1, y1, x2, y2, x3, y3);

        path1.d2x[i] = cubic_bezier_2nd_deriv(t, x0, x1, x2, x3);
        path1.d2y[i] = cubic_bezier_2nd_deriv(t, y0, y1, y2, y3);
        path1.d2slope[i] = path1.d2y[i] / path1.d2x[i];*/

        // path1.d2x[i] = path1.d2x[path1.fidelity];
        // path1.d2y[i] = path1.d2y[path1.fidelity];
    }
    this->deltaTheta[numberOfPoints] = 0;
    //int endTime = Brain.timer(vex::msec);
    //printf("\nendTime: %d", endTime);
    //printf("\nTime: %d", endTime - startTime);
    //std::cout << "\n"
    //          << std::flush;
}

void Path::printPath()
{
    printf("\n\nGiven Points: \n");
    printf("(%.2f, %.2f),(%.2f, %.2f),(%.2f, %.2f),(%.2f, %.2f),\n", x0, y0, x1, y1, x2, y2, x3, y3);
    // wait(1500, vex::msec);
    std::cout << "\n"
              << std::flush;

    int p = 0;
    for (int i = 0; i <= numberOfPoints; i++)
    {
        printf("(%.2f, %.2f),", this->x[i], this->y[i]);
        // printf("(%.2f),", this->deltaTheta[i]);
        if (p > 4)
        {
            p = 0;
            printf("\n");
        }
        p++;
        std::cout << std::flush;
    }

    printf("\n");
    printf("curve Length: %.2f", this->length);

    std::cout << "\n"
              << std::flush;
}
