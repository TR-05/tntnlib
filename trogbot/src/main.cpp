#include "vex.h"
#include "../tntnlibrary/include/api.h"
#include "autos.h"
#include <iostream>
using namespace tntnlib;
vex::competition Competition;
vex::brain Brain;

vex::vision::signature SIG_1(3, 8099, 8893, 8496, -1505, -949, -1227, 2.5, 0);
vex::vision vision1(vex::PORT13, 50, SIG_1);

/* tntnlib robot Config */
MotorGroup leftMotors(vex::gearSetting::ratio6_1, 360, -8, -10);
MotorGroup rightMotors(vex::gearSetting::ratio6_1, 360, 18, 19);
TrackingWheel horizontal(Brain.ThreeWirePort.G, Omniwheel::NEW_275, 0.002292, 1);
TrackingWheel vertical(Brain.ThreeWirePort.E, Omniwheel::NEW_275, -0.253611, 1);
Gyro imu(1, 1.010357);
/* chassis and controllers (DO NOT CHANGE NAMES) */
ControllerSettings linearSettings(.6, 0, 3.5, 0, 0, 12);
ControllerSettings angularSettings(.25, 0.01, 2.0, 10, 2, 12);
Drivetrain drivetrain(&leftMotors, &rightMotors, 10.0, Omniwheel::OLD_325, 360, 8);
OdomSensors sensors(&vertical, nullptr, &horizontal, nullptr, &imu);
Chassis chassis(drivetrain, linearSettings, angularSettings, sensors);
// Flywheel flywheel(ratio6_1, 3600, 10, 0, 0.8, .25, -8, -10);
// MotorGroup intake(vex::gearSetting::ratio6_1, 360, -8, -10);

/* End of tntnlib Robot Config */

/* data logger idk where to put :/ */

// width is 0-315 -> half is 157
float halfWidth = 157;
// height is 0-211 -> half is 105
float halfHeight = 105;
float cameraObjectX = 0, cameraObjectY = 0;
float cameraObjectWidth = 0, cameraObjectHeight = 0;
float leftDrivePower = 0, rightDrivePower = 0;
int logger()
{
  while (true)
  {
    Pose current(chassis.getPose(false));
    // printf("SX: %.2f, SR: %.2f, IMU: %.2f ", chassis.sensors.horizontal1 != nullptr ? chassis.sensors.horizontal1->getDistance() : 0, chassis.sensors.vertical1 != nullptr ? chassis.sensors.vertical1->getDistance() : 0, chassis.sensors.gyro != nullptr ? chassis.sensors.gyro->rotation() : 0);
    // printf("  X: %.2f,  Y: %.2f,  H: %.2f   T: %.2f ET:%.2f\n", current.x, current.y, current.theta, getTime(), totalTime / 1000.0);

    // printf("  X: %.2f,  Y: %.2f,  H: %.2f   T: %.2f ET:%.2f\n", current.x, current.y, current.theta, getTime(), totalTime / 1000.0);

    Brain.Screen.clearLine(1);
    Brain.Screen.clearLine(2);
    Brain.Screen.clearScreen(vex::color(255, 20, 0));
    Brain.Screen.setFillColor(vex::color(255, 20, 0));
    Brain.Screen.setPenColor(vex::color(0, 0, 0));
    vision1.takeSnapshot(SIG_1);
    Brain.Screen.setCursor(1, 1);
    printf("On: %d", vision1.installed());
    if (vision1.objectCount > 0)
    {
      cameraObjectX = ema(-1 * (vision1.largestObject.centerX - halfWidth), cameraObjectX, .9);
      cameraObjectY = ema(-1 * (vision1.largestObject.centerY - halfWidth), cameraObjectY, .9);
      cameraObjectWidth = vision1.largestObject.width;
      cameraObjectHeight = vision1.largestObject.height;

      Brain.Screen.print("X:%.3f, Y:%.3f", cameraObjectX, cameraObjectY);
      Brain.Screen.setCursor(2, 0);
      Brain.Screen.print("w:%.2f, h:%.2f", cameraObjectWidth, cameraObjectHeight);

      if (Controller.ButtonR1.pressing())
      {
        leftDrivePower = clamp(cameraObjectX * .2, -12, 12);
        rightDrivePower = clamp(-cameraObjectX * .2, -12, 12);
        printf(" X: %.2f,  Y: %.2f", cameraObjectX, cameraObjectY);
        printf("  L: %.2f,  R: %.2f\n", defaultDriveCurve(leftDrivePower, 100), defaultDriveCurve(rightDrivePower, 100));
      }

      else
      {
        printf(" X: %.2f,  Y: %.2f\n", cameraObjectX, cameraObjectY);
        leftDrivePower = 0;
        rightDrivePower = 0;
      }
    }
    else
    {
      Brain.Screen.print("Not Found");
      printf("None\n");
    }

    // Brain.Screen.print("X:%6.2f, Y:%6.2f, H:%6.2f", current.x, current.y, current.theta);
    vex::wait(20, vex::msec);
  }
  return 0;
}

/* runs when program first starts */
void pre_auton()
{
  Brain.Screen.clearScreen(vex::color(255, 20, 0));

  vex::task log(logger);
  printf("Entered pre_auton\n");
  chassis.initialize(true, 0, 0, 0);
}

/* runs on comp switch autonomous */
void autonomous()
{
  programming_skills();
}

/* runs on comp switch driver */
void usercontrol()
{
  printf("Entered Driver\n");
  chassis.stateMachineOff();
  // intake.setBrakeType(vex::brakeType::brake);
  while (1)
  {
    chassis.tank(Controller.Axis3.position()*.12 + leftDrivePower, Controller.Axis2.position()*.12 + rightDrivePower, 100); // tank (the best drive style)
    // intake.driverTwoButton(Controller.ButtonL1.pressing(), Controller.ButtonL2.pressing(), 12, -12);
    // flywheel.spinVolts(6);
    // intake.driverToggle(Controller.ButtonA.pressing(), 3);
    //  chassis.arcade(Controller.Axis3.position(), Controller.Axis4.position(), 0); //single stick arcade
    //  chassis.arcade(Controller.Axis3.position(), Controller.Axis1.position(), 0); //split arcade
    vex::wait(10.0, vex::msec);
  }
}

/* comp switch callbacks */
int main()
{
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
  pre_auton();

  while (true)
  {
    vex::wait(100, vex::msec);
  }
}
