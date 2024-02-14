#pragma once

#include <vector>
#include "vex.h"
namespace tntnlib
{
    // uses 4 points and a number of points to generate a path
    class Path
    {
        // The Path class represents a path defined by a number of points.
    public:
        // Constructor for the Path class.
        // Initializes the number of points and creates vectors of that size for each attribute.
        Path(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, int numberOfPoints = 100)
            : x0(x0),
              y0(y0),
              x1(x1),
              y1(y1),
              x2(x2),
              y2(y2),
              x3(x3),
              y3(y3),
              numberOfPoints(numberOfPoints - 1),
              x(numberOfPoints),
              y(numberOfPoints),
              theta(numberOfPoints),
              deltaTheta(numberOfPoints),
              t(numberOfPoints),
              segmentLength(numberOfPoints),
              segmentCurvature(numberOfPoints)
        {
            generatePath();
        }
        float x0, y0, x1, y1, x2, y2, x3, y3;
        int numberOfPoints;
        std::vector<float> x;
        std::vector<float> y;
        std::vector<float> theta;
        std::vector<float> deltaTheta;
        std::vector<float> t;
        std::vector<float> segmentLength;
        std::vector<float> segmentCurvature;

        float length{0}; // The total length of the path
        void printPath();
        Path operator+(const Path &other);

    private:
        float cubicInterpolate(float p0, float p1, float p2, float p3, float t);
        float cubicInterpolateD1(float p0, float p1, float p2, float p3, float t);
        float cubicInterpolateD2(float p0, float p1, float p2, float p3, float t); //idk why you would need this but yk
        float cubicInterpolateDerivativeDegrees(float t);
        void generatePath();
    };
} // namespace tntnlib
