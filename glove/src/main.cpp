#include "vex.h"
#include "../tntnlibrary/include/api.h"
#include "autos.h"
#include <iostream>
using namespace tntnlib;
vex::competition Competition;
vex::brain Brain;

/* tntnlib robot Config */
MotorGroup leftMotors(vex::gearSetting::ratio6_1, 450, -7, 8, -9, -10);
// MotorGroup leftMotors(vex::gearSetting::ratio6_1, 11);
// MotorGroup rightMotors(vex::gearSetting::ratio6_1, 11);

// MotorGroup leftMotors2(vex::gearSetting::ratio6_1, -7);
// MotorGroup rightMotors2(vex::gearSetting::ratio6_1, 1);
MotorGroup rightMotors(vex::gearSetting::ratio6_1, 450, 1, 2, -3, 4);
// MotorGroup rightMotors(vex::gearSetting::ratio6_1, 4);
// vex::motor leftTest(vex::PORT7, vex::ratio6_1, true);
// vex::motor rightTest(vex::PORT1, vex::ratio6_1, false);

/* tracking wheels and gyro */
TrackingWheel horizontal(Brain.ThreeWirePort.A, Omniwheel::NEW_275, 0.002292, 1);
TrackingWheel vertical(Brain.ThreeWirePort.C, Omniwheel::NEW_275, -0.253611, 1);
Gyro imu(12, 1.010357);

/* chassis and controllers */
ControllerSettings linearSettings(.6, 0, 3.5, 0, 0, 12);
ControllerSettings angularSettings(.25, 0.01, 2.0, 10, 2, 12);
Drivetrain drivetrain(&leftMotors, &rightMotors, 10.0, Omniwheel::NEW_275, 450, 8);
OdomSensors sensors(&vertical, nullptr, &horizontal, nullptr, &imu);
Chassis chassis(drivetrain, linearSettings, angularSettings, sensors);
/* End of tntnlib Robot Config */

MotorGroup intake(vex::gearSetting::ratio6_1, 600, -11);
vex::digital_out left_wing(Brain.ThreeWirePort.F);
vex::digital_out right_wing(Brain.ThreeWirePort.E);
vex::digital_out hang(Brain.ThreeWirePort.G);

/* data logger idk where to put :/ */
int logger()
{
  while (true)
  {
    Pose current(chassis.getPose(false));
    // printf("SX: %.2f, SR: %.2f, IMU: %.2f ", chassis.sensors.horizontal1 != nullptr ? chassis.sensors.horizontal1->getDistance() : 0, chassis.sensors.vertical1 != nullptr ? chassis.sensors.vertical1->getDistance() : 0, chassis.sensors.gyro != nullptr ? chassis.sensors.gyro->rotation() : 0);
    // printf("  X: %.2f,  Y: %.2f,  H: %.2f   T: %.2f ET:%.2f\n", current.x, current.y, current.theta, getTime(), totalTime / 1000.0);
    Brain.Screen.clearLine();
    Brain.Screen.print("X:%6.2f, Y:%6.2f, H:%6.2f", current.x, current.y, current.theta);
    vex::wait(50, vex::msec);
  }
  return 0;
}

/* runs when program first starts */
void pre_auton()
{
  vex::task log(logger);
  printf("Entered pre_auton\n");
  chassis.initialize(true, 0, 0, 0);
}

/* runs on comp switch autonomous */
void autonomous()
{
  programming_skills();
}

void toggleLeftWing()
{
  left_wing.set(!left_wing.value());
}
void toggleRightWing()
{
  right_wing.set(!right_wing.value());
}
void wings()
{
  left_wing.set(!left_wing.value());
  right_wing.set(!left_wing.value());
}

/* runs on comp switch driver */
void usercontrol()
{
  printf("Entered Driver\n");
  chassis.stateMachineOff();

  bool L1pressed;
  bool A;
  bool Y;
  while (1)
  {

    if (Controller.ButtonL1.pressing())
    {
      if (!L1pressed)
      {
        wings();
      }
      L1pressed = true;
    }
    else
    {
      L1pressed = false;
    }

    if (Controller.ButtonY.pressing())
    {
      if (!Y)
      {
        toggleLeftWing();
      }
      Y = true;
    }
    else
    {
      Y = false;
    }

    if (Controller.ButtonA.pressing())
    {
      if (!A)
      {
        toggleRightWing();
      }
      A = true;
    }
    else
    {
      A = false;
    }

    if (Controller.ButtonB.pressing())
    {
      hang.set(1);
    }
    if (Controller.ButtonX.pressing())
    {
      hang.set(0);
    }

    intake.driverTwoButton(Controller.ButtonR1.pressing(), Controller.ButtonR2.pressing(), 12, -12);
    intake.setBrakeType(vex::brakeType::brake);
    // chassis.tank(Controller.Axis3.position(), Controller.Axis2.position(), 100); // tank (the best drive style)
    chassis.arcade(Controller.Axis3.position()*.12, Controller.Axis4.position()*.12, 0); // single stick arcade
    // chassis.arcade(Controller.Axis3.position(), Controller.Axis1.position(), 0); //split arcade
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
