#include "vex.h"
#include "../tntnlibrary/include/api.h"
#include "autos.h"
#include <iostream>
using namespace vex;
competition Competition;
brain Brain;

/* tntnlib robot Config */

/* drive motors */
motor LS_bottom = motor(PORT9, ratio18_1, true);
motor LS_middle = motor(PORT8, ratio18_1, false);
motor LS_top = motor(PORT7, ratio18_1, true);
motor LS_front = motor(PORT10, ratio18_1, true);

motor RS_bottom = motor(PORT2, ratio18_1, false);
motor RS_middle = motor(PORT3, ratio18_1, true);
motor RS_top = motor(PORT4, ratio18_1, false);
motor RS_front = motor(PORT1, ratio18_1, false);

motor_group leftMotors = motor_group(LS_bottom, LS_middle, LS_top, LS_front);
motor_group rightMotors = motor_group(RS_bottom, RS_middle, RS_top, RS_front);
/* tracking wheels and gyro */
tntnlib::TrackingWheel horizontal(Brain.ThreeWirePort.A, tntnlib::Omniwheel::NEW_275, 0.002292, 1);
tntnlib::TrackingWheel vertical(Brain.ThreeWirePort.C, tntnlib::Omniwheel::NEW_275, -0.253611, 1);
tntnlib::Gyro imu(12, 1.010357);

/* chassis and controllers */
tntnlib::ControllerSettings tntnlib::linearSettings(.6, 0, 3.5, 0, 0, 12);
tntnlib::ControllerSettings tntnlib::angularSettings(.25, 0.01, 2.0, 10, 2, 12);
tntnlib::Drivetrain tntnlib::drivetrain(&leftMotors, &rightMotors, 10.0, tntnlib::Omniwheel::OLD_325, 360, 8);
tntnlib::OdomSensors tntnlib::sensors(&vertical, nullptr, &horizontal, nullptr, &imu);
/* End of tntnlib Robot Config */

motor intake = motor(PORT11, ratio6_1, true);
digital_out left_wing = digital_out(Brain.ThreeWirePort.F);
digital_out right_wing = digital_out(Brain.ThreeWirePort.E);

/* data logger idk where to put :/ */
int logger()
{
  while (true)
  {
    tntnlib::Pose current(chassis.getPose(false));
    printf("SX:%6.2f, SR:%6.2f, IMU:%6.2f ", tntnlib::sensors.horizontal1->getDistance(), tntnlib::sensors.vertical1->getDistance(), tntnlib::sensors.gyro != nullptr ? tntnlib::sensors.gyro->rotation() : 0);
    printf("X:%9.2f, Y:%9.2f, H:%9.2f BH:%9.2f\n", current.x, current.y, current.theta, fmod(current.theta, 360));
    Brain.Screen.clearLine();
    Brain.Screen.print("X:%6.2f, Y:%6.2f, H:%6.2f", current.x, current.y, current.theta);
    wait(50, msec);
  }
  return 0;
}

/* runs when program first starts */
void pre_auton()
{
  task log(logger);
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
    float pow = (Controller.ButtonR1.pressing() - Controller.ButtonR2.pressing()) * 12;
    intake.spin(fwd, pow, volt);
    intake.setStopping(brake);
    //chassis.tank(Controller.Axis3.position(), Controller.Axis2.position(), 100); // tank (the best drive style)
    chassis.arcade(Controller.Axis3.position(), Controller.Axis4.position(), 0); //single stick arcade
    // chassis.arcade(Controller.Axis3.position(), Controller.Axis1.position(), 0); //split arcade
    wait(10.0, msec);
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
    wait(100, msec);
  }
}
