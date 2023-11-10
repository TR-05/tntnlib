/**
 * @file src/tntnlib/util.cpp
 * @author Trevor Ruttan
 * @brief Pose class declarations
 * @version 0.0
 * @date 2023-10-12
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <math.h>
#include <vector>
#include <string.h>
#include "tntnlib/pose.h"
#include "tntnlib/util.h"
#include "tntnlib/devices/defaultDevices.h"

/**
 * @brief Slew rate limiter
 *
 * @param target target value
 * @param current current value
 * @param maxChange maximum change. No maximum if set to 0
 * @return float - the limited value
 */
float tntnlib::slew(float target, float current, float maxChange)
{
    float change = target - current;
    if (maxChange == 0)
        return target;
    if (change > maxChange)
        change = maxChange;
    else if (change < -maxChange)
        change = -maxChange;
    return current + change;
}

/**
 * @brief Convert radians to degrees
 *
 * @param rad radians
 * @return float degrees
 */
float tntnlib::radToDeg(float rad) { return rad * 180 / M_PI; }

/**
 * @brief Convert degrees to radians
 *
 * @param deg degrees
 * @return float radians
 */
float tntnlib::degToRad(float deg) { return deg * M_PI / 180; }

float tntnlib::degToStandardFormRad(float deg) { return M_PI_2 - (deg * M_PI / 180); }
float tntnlib::StandardFormRadToDeg(float rad) { return (M_PI_2 - rad) * 180 / M_PI; }

/**
 * @brief Calculate the error between 2 angles. Useful when calculating the error between 2 headings
 *
 * @param angle1
 * @param angle2
 * @param radians true if angle is in radians, false if not. False by default
 * @return float wrapped angle
 */
float tntnlib::angleError(float angle1, float angle2, bool radians)
{
    float max = radians ? 2 * M_PI : 360;
    float half = radians ? M_PI : 180;
    angle1 = fmod(angle1, max);
    angle2 = fmod(angle2, max);
    float error = angle1 - angle2;
    if (error > half)
        error -= max;
    else if (error < -half)
        error += max;
    return error;
}

/**
 * @brief Return the sign of a number
 *
 * @param x the number to get the sign of
 * @return int - -1 if negative, 1 if positive
 */
int tntnlib::sgn(float x)
{
    if (x < 0)
        return -1;
    else
        return 1;
}

/**
 * @brief Return the average of a vector of numbers
 *
 * @param values
 * @return float
 */
float tntnlib::avg(std::vector<float> values)
{
    float sum = 0;
    for (float value : values)
    {
        sum += value;
    }
    return sum / values.size();
}

/**
 * @brief Exponential moving average
 *
 * @param current current measurement
 * @param previous previous output
 * @param smooth smoothing factor (0-1). 1 means no smoothing, 0 means no change
 * @return float - the smoothed output
 */
float tntnlib::ema(float current, float previous, float smooth)
{
    return (current * smooth) + (previous * (1 - smooth));
}

float tntnlib::clamp(float input, float min, float max)
{
    float output;
    if (input > max)
        output = max;
    else if (input < min)
        output = min;
    else
    {
        output = input;
    }
    return output;
}

float tntnlib::time(bool millis)
{
    float t = Brain.timer(vex::msec);
    return millis ? t : t / 1000.0;
}
/**
 * Finds the curvature of a circle which intersects 2 points, and is tangent to the first point
 *
 * Inspired by: https://www.chiefdelphi.com/t/paper-implementation-of-the-adaptive-pure-pursuit-controller/166552
 */
float tntnlib::getCurvature(Pose p1, Pose p2)
{
    // calculate whether the pose is on the left or right side of the circle
    float side = sgn(sin(p1.theta) * (p2.x - p1.x) - cos(p1.theta) * (p2.y - p1.y));
    // calculate center point and radius
    float a = -tan(p1.theta);
    float c = tan(p1.theta) * p1.x - p1.y;
    float x = fabs(a * p2.x + p2.y + c) / sqrt((a * a) + 1);
    float d = p1.distance(p2);

    // return curvature
    return side * ((2 * x) / (d * d));
}

/**
 * Finds the closest waypoint in a vector of waypoints to a target point
 *
 * Calculates the distance between the target pose and each waypoint in the vector.
 * The waypoint with the smallest distance is returned.
 *
 * The function only calculates the square of the distance, because the exact distance is not needed.
 * It only needs to know what waypoint is closest.
 */
tntnlib::Waypoint tntnlib::closestWaypoint(const std::vector<Waypoint> &waypoints, const Pose &target)
{
    Waypoint closest = waypoints[0];
    float dist = pow(target.x - closest.x, 2) + pow(target.y - closest.y, 2);

    // loop through all path points
    for (const Waypoint &wp : waypoints)
    {
        const float dist2 = pow(target.x - wp.x, 2) + pow(target.y - wp.y, 2);
        if (dist2 < dist)
        {
            closest = wp;
            dist = dist2;
        }
    }

    return closest;
}

/**
 * Find the intersection between a line segment and a circle given its center and radius
 *
 * Inspired by: https://stackoverflow.com/a/1084899
 *
 * It uses the quadratic formula to calculate the intersection points.
 * If there are no intersections, it returns the center of the circle.
 * If there are multiple intersections, it returns the first one.
 */
tntnlib::Pose tntnlib::circleLineIntersect(Pose p1, Pose p2, Pose center, float radius)
{
    // uses the quadratic formula to calculate intersection points
    tntnlib::Pose d = p2 - p1;
    tntnlib::Pose f = p1 - center;
    float a = d * d;
    float b = 2 * (f * d);
    float c = (f * f) - pow(radius, 2);
    float discriminant = pow(b, 2) - 4 * a * c;

    // if a possible intersection was found
    if (discriminant >= 0)
    {
        discriminant = sqrt(discriminant);
        float t1 = (-b - discriminant) / (2 * a);
        float t2 = (-b + discriminant) / (2 * a);

        // prioritize first intersection
        float t = -1;
        if (t2 >= 0 && t2 <= 1)
            t = t2;
        if (t1 >= 0 && t1 <= 1)
            t = t1;
        if (t == -1)
            return center;

        // calculate x and y values of intersection point
        return p1.lerp(p2, t);
    }

    // no intersection found
    return center;
}
