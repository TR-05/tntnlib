#include "vex.h"
#include "../tntnlibrary/include/api.h"
#include "../tntnlibrary/include/drivetrain/movements/turn.h"
#include "autos.h"
#include "vision.h"
#include <iostream>

using namespace tntnlib;

// width is 0-315 -> half is 157
float halfWidth = 157;
// height is 0-211 -> half is 105
float halfHeight = 105;
float cameraObjectX = 0, cameraObjectY = 0;
float cameraObjectWidth = 0, cameraObjectHeight = 0;
float leftDrivePower = 0, rightDrivePower = 0;
float kVision = 20;
float minArea = 0;

float offset = -7.5;
void screenPrint()
{
    Brain.Screen.clearScreen(vex::color::cyan);
    Brain.Screen.setFillColor(vex::color::cyan);
    Brain.Screen.setPenColor(vex::color::black);
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("X:%.3f, Y:%.3f", cameraObjectX, cameraObjectY);
    Brain.Screen.setCursor(2, 1);
    Brain.Screen.print("w:%.2f, h:%.2f", cameraObjectWidth, cameraObjectHeight);
}

float visionOutput = 0;

float visionX()
{
    vision1.takeSnapshot(SIG_1);
    if (vision1.objectCount > 0)
    {
        cameraObjectX = ema(-1 * (vision1.largestObject.centerX - halfWidth) / halfWidth, cameraObjectX, .2);
        cameraObjectY = ema(-1 * (vision1.largestObject.centerY - halfWidth) / halfHeight, cameraObjectY, .2);

        cameraObjectWidth = vision1.largestObject.width;
        cameraObjectHeight = vision1.largestObject.height;
        // use area for certainty?
        float area = cameraObjectHeight * cameraObjectWidth;
        visionOutput = cameraObjectX * kVision + offset;
        if (area < minArea)
            visionOutput = 0;
        return visionOutput;
    }
    visionOutput = 0;
    return 0;
}
float visionPower()
{
    angularPID.setGains(0, 0, angularSettings.kP*3, angularSettings.kI*1.5, angularSettings.kD);
    angularPID.setIntegral(15, 4);
    // angularPID.reset();
    float visionChange = visionX();
    screenPrint();

    // calculate error
    float error = visionChange;
    float output = angularPID.update(error, 0);
    // cap the speed
    output = clamp(output, -6, 6);
    float volts = output;
    if (visionChange != 0) ;//printf("visionOutput: %.2f, Volts: %.2f\n", visionOutput, volts);
    else {
        //printf("not found: %.2f\n", visionOutput);
    }
    return volts;
}
