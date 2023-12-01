#include "vex.h"
#include "../tntnlibrary/include/api.h"
#include "autos.h"
#include <iostream>
using namespace vex;
competition Competition;
brain Brain;

/* tntnlib robot Config */

/* drive motors */
motor ls_front = motor(8  -1, ratio6_1, true);
motor ls_back = motor(10  -1, ratio6_1, true);
motor rs_front = motor(PORT18, ratio6_1, false);
motor rs_back = motor(PORT19, ratio6_1, false);

motor_group leftMotors = motor_group(ls_front, ls_back);
motor_group rightMotors = motor_group(rs_front, rs_back);

/* tracking wheels and gyro */
tntnlib::TrackingWheel horizontal(Brain.ThreeWirePort.G, tntnlib::Omniwheel::NEW_275, 0.002292, 1);
tntnlib::TrackingWheel vertical(Brain.ThreeWirePort.E, tntnlib::Omniwheel::NEW_275, -0.253611, 1);

tntnlib::Gyro imu(1, 1.010357);

/* chassis and controllers (DO NOT CHANGE NAMES)*/
tntnlib::ControllerSettings tntnlib::linearSettings(.6, 0, 3.5, 0, 0, 12);
tntnlib::ControllerSettings tntnlib::angularSettings(.25, 0.01, 2.0, 10, 2, 12);
tntnlib::Drivetrain tntnlib::drivetrain(&leftMotors, &rightMotors, 10.0, tntnlib::Omniwheel::OLD_325, 360, 8);
tntnlib::OdomSensors tntnlib::sensors(&vertical, nullptr, &horizontal, nullptr, &imu);
/* End of tntnlib Robot Config */
std::vector<vex::motor> flywheelMotors = {ls_front, ls_back, rs_front, rs_back};
tntnlib::Flywheel flywheel(&flywheelMotors, 600, 3600);
/* data logger idk where to put :/ */
int logger()
{
  while (true)
  {
    tntnlib::Pose current(chassis.getPose(false));
    printf("SX: %.2f, SR: %.2f, IMU: %.2f ", tntnlib::sensors.horizontal1 != nullptr ? tntnlib::sensors.horizontal1->getDistance() : 0, tntnlib::sensors.vertical1 != nullptr ? tntnlib::sensors.vertical1->getDistance() : 0, tntnlib::sensors.gyro != nullptr ? tntnlib::sensors.gyro->rotation() : 0);
    printf("  X: %.2f,  Y: %.2f,  H: %.2f   BH: %.2f\n", current.x, current.y, current.theta, fmod(current.theta, 360));
    Brain.Screen.clearLine();
    Brain.Screen.print("X:%6.2f, Y:%6.2f, H:%6.2f", current.x, current.y, current.theta);
    wait(50, msec);
  }
  return 0;
}

/* runs when program first starts */
void pre_auton()
{
  // task log(logger);
  printf("Entered pre_auton\n");
  chassis.initialize(true, 0, 0, 0);
  flywheel.settings(0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 0);
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

  while (1)
  {
    chassis.tank(Controller.Axis3.position(), Controller.Axis2.position(), 100); // tank (the best drive style)
    // chassis.arcade(Controller.Axis3.position(), Controller.Axis4.position(), 0); //single stick arcade
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
