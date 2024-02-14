#pragma once

#include "../tntnlibrary/include/drivetrain/sensors/gyro/basegyro.h"
#include "vex.h"
#include <cstdint>
#include <memory>

namespace tntnlib {

class Gyro {
  public:
    /**
     * @brief Creates a new gyro using a vex inertial sensor on the specified
     * port
     *
     * @param encoder unique ptr to the custom encoder to be used
     * @param diameter the diameter of the wheel, in inches
     * @param offset distance between the wheel and the tracking center, in
     * inches
     */
    Gyro(int port, float multiplier);

    /**
     * @brief set the gyro to input degrees
     *
     */
    void setTo(float deg);
    /**
     * @brief Get the rotational angle of the gyro, in degrees
     *
     * @return float rotational angle of the gyro, in degrees
     */
    float rotation();
    /**
     * @brief Gets the gyro's constant multiplier
     *
     * @return float multiplier
     */
    float getMultiplier() const;

    void calibrate();

    bool isCalibrating();

  private:
    std::shared_ptr<BaseGyro> gyro;
    const float multiplier;
};
} // namespace tntnlib