/**
 * @file src/troglib/pose.cpp
 * @author Trevor Ruttan
 * @brief Pose class declarations
 * @version 0.0
 * @date 2023-10-12
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <math.h>
#include "troglib/pose.h"
/**
 * @brief Create a new pose
 *
 * @param x component
 * @param y component
 * @param theta heading. Defaults to 0
 */
troglib::Pose::Pose(float x, float y, float theta) {
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
troglib::Pose troglib::Pose::operator+(const troglib::Pose& other) {
    this->x += other.x, this->y += other.y, this->theta += other.theta;
    return troglib::Pose(this->x, this->y, this->theta);
}

/**
 * @brief Subtract a pose from this pose
 *
 * @param other other pose
 * @return Pose
 */
troglib::Pose troglib::Pose::operator-(const troglib::Pose& other) {
    return troglib::Pose(this->x - other.x, this->y - other.y, this->theta);
}

/**
 * @brief Multiply a pose by this pose
 *
 * @param other other pose
 * @return Pose
 */
float troglib::Pose::operator*(const troglib::Pose& other) { return this->x * other.x + this->y * other.y; }

/**
 * @brief Multiply a pose by a float
 *
 * @param other float
 * @return Pose
 */
troglib::Pose troglib::Pose::operator*(const float& other) {
    return troglib::Pose(this->x * other, this->y * other, this->theta);
}

/**
 * @brief Divide a pose by a float
 *
 * @param other float
 * @return Pose
 */
troglib::Pose troglib::Pose::operator/(const float& other) {
    return troglib::Pose(this->x / other, this->y / other, this->theta);
}

/**
 * @brief Check if two poses are equal
 *
 * @param other the other pose
 * @return bool
 */
bool troglib::Pose::operator==(const Pose& other) {
    return this->x == other.x && this->y == other.y && this->theta == other.theta;
}

/**
 * @brief Check if two poses are not equal
 *
 * @param other the other pose
 * @return bool
 */
bool troglib::Pose::operator!=(const Pose& other) {
    return this->x != other.x || this->y != other.y || this->theta != other.theta;
}

/**
 * @brief Linearly interpolate between two poses
 *
 * @param other the other pose
 * @param t t value
 * @return Pose
 */
troglib::Pose troglib::Pose::lerp(troglib::Pose other, float t) {
    return troglib::Pose(this->x + (other.x - this->x) * t, this->y + (other.y - this->y) * t, this->theta);
}

/**
 * @brief Get the distance between two poses
 *
 * @param other the other pose
 * @return float
 */
float troglib::Pose::distance(troglib::Pose other) { return hypot(this->x - other.x, this->y - other.y); }

/**
 * @brief Get the angle between two poses
 *
 * @param other the other pose
 * @return float in radians
 */
float troglib::Pose::angle(troglib::Pose other) { return atan2(other.y - this->y, other.x - this->x); }

/**
 * @brief Rotate a pose by an angle
 *
 * @param angle angle in radians
 * @return Pose
 */
troglib::Pose troglib::Pose::rotate(float angle) {
    return troglib::Pose(this->x * cos(angle) - this->y * sin(angle),
                        this->x * sin(angle) + this->y * cos(angle), this->theta);
}
