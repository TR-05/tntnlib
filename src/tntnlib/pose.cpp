/**
 * @file src/tntnlib/pose.cpp
 * @author Trevor Ruttan
 * @brief Pose class declarations
 * @version 0.0
 * @date 2023-10-12
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <math.h>
#include "tntnlib/pose.h"
/**
 * @brief Create a new pose
 *
 * @param x component
 * @param y component
 * @param theta heading. Defaults to 0
 */
tntnlib::Pose::Pose(float x, float y, float theta) {
    this->x = x;
    this->y = y;
    this->theta = theta;
}

/**
 * @brief Add a pose to this pose
 *
 * @param other other pose
 * @return Pose
 */
tntnlib::Pose tntnlib::Pose::operator+(const tntnlib::Pose& other) {
    this->x += other.x, this->y += other.y, this->theta += other.theta;
    return tntnlib::Pose(this->x, this->y, this->theta);
}

/**
 * @brief Subtract a pose from this pose
 *
 * @param other other pose
 * @return Pose
 */
tntnlib::Pose tntnlib::Pose::operator-(const tntnlib::Pose& other) {
    return tntnlib::Pose(this->x - other.x, this->y - other.y, this->theta);
}

/**
 * @brief Multiply a pose by this pose
 *
 * @param other other pose
 * @return Pose
 */
float tntnlib::Pose::operator*(const tntnlib::Pose& other) { return this->x * other.x + this->y * other.y; }

/**
 * @brief Multiply a pose by a float
 *
 * @param other float
 * @return Pose
 */
tntnlib::Pose tntnlib::Pose::operator*(const float& other) {
    return tntnlib::Pose(this->x * other, this->y * other, this->theta);
}

/**
 * @brief Divide a pose by a float
 *
 * @param other float
 * @return Pose
 */
tntnlib::Pose tntnlib::Pose::operator/(const float& other) {
    return tntnlib::Pose(this->x / other, this->y / other, this->theta);
}

/**
 * @brief Check if two poses are equal
 *
 * @param other the other pose
 * @return bool
 */
bool tntnlib::Pose::operator==(const Pose& other) {
    return this->x == other.x && this->y == other.y && this->theta == other.theta;
}

/**
 * @brief Check if two poses are not equal
 *
 * @param other the other pose
 * @return bool
 */
bool tntnlib::Pose::operator!=(const Pose& other) {
    return this->x != other.x || this->y != other.y || this->theta != other.theta;
}

/**
 * @brief Linearly interpolate between two poses
 *
 * @param other the other pose
 * @param t t value
 * @return Pose
 */
tntnlib::Pose tntnlib::Pose::lerp(tntnlib::Pose other, float t) {
    return tntnlib::Pose(this->x + (other.x - this->x) * t, this->y + (other.y - this->y) * t, this->theta);
}

/**
 * @brief Get the distance between two poses
 *
 * @param other the other pose
 * @return float
 */
float tntnlib::Pose::distance(tntnlib::Pose other) { return hypot(this->x - other.x, this->y - other.y); }

/**
 * @brief Get the angle between two poses
 *
 * @param other the other pose
 * @return float in radians
 */
float tntnlib::Pose::angle(tntnlib::Pose other) { return atan2(other.y - this->y, other.x - this->x); }

/**
 * @brief Rotate a pose by an angle
 *
 * @param angle angle in radians
 * @return Pose
 */
tntnlib::Pose tntnlib::Pose::rotate(float angle) {
    return tntnlib::Pose(this->x * cos(angle) - this->y * sin(angle),
                        this->x * sin(angle) + this->y * cos(angle), this->theta);
}
