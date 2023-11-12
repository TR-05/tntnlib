#include "vex.h"
#include "tntnlib/api.h"
#include <iostream>
using namespace vex;
// A global instance of competition
competition Competition;
brain Brain;

motor ls_front = motor(PORT8, ratio6_1, true);
motor ls_back = motor(PORT10, ratio6_1, true);
motor rs_front = motor(PORT18, ratio6_1, false);
motor rs_back = motor(PORT19, ratio6_1, false);
motor_group leftMotors = motor_group(ls_front, ls_back);
motor_group rightMotors = motor_group(rs_front, rs_back);

tntnlib::TrackingWheel horizontal(Brain.ThreeWirePort.G, tntnlib::Omniwheel::NEW_275, 0.0586285, 1);
tntnlib::TrackingWheel vertical(Brain.ThreeWirePort.E, tntnlib::Omniwheel::NEW_275, -0.0164235, 1);
inertial imu = inertial(PORT1);

tntnlib::Drivetrain_t drivebase{&leftMotors, &rightMotors, 10.0, tntnlib::Omniwheel::OLD_325, 360, 8};
tntnlib::ChassisController_t linearController{1, 0, 3, 0, 0, 0, 100, 3, 500, 12};
tntnlib::ChassisController_t angularController{.25, 0.009, 2.0, 10, 2, .5, 1, 3, 500, 12};
tntnlib::OdomSensors_t sensors{&vertical, nullptr, &horizontal, nullptr, &imu};
// MUST BE NAMED chassis OR GET TREVOR TO ADD IT TO chassis.cpp and chassis.h (odomLoop and bottom of .h)
tntnlib::Chassis chassis(drivebase, linearController, angularController, sensors);

int logger()
{
  while (true)
  {
    tntnlib::Pose current(chassis.getPose(false));
    //printf("\n\nSX:%.2f, SR:%.2f, IMU:%.2f ", sensors.horizontal1->getDistance(), sensors.vertical1->getDistance(), sensors.imu->rotation());
    //printf("X:%.2f, Y:%.2f, H:%.2f\n", current.x, current.y, current.theta);
    Brain.Screen.clearLine();
    Brain.Screen.print("X:%.2f, Y:%.2f, H:%.2f", current.x, current.y, current.theta);
    wait(50, msec);
  }

  return 0;
}

void pre_auton(void)
{
  task log(logger);
  printf("Entered pre_auton\n");
  chassis.initialize(true, 0, 0, 0);
}

void autonomous(void)
{
  printf("Entered Auto\n");
  // chassis.initialize(false, 0,0,0);
  chassis.stateMachineOn();
  float akp = angularController.kP;
  float aki = angularController.kI;
  float akd = angularController.kD;
  float lkp = linearController.kP;
  float lki = linearController.kI;
  float lkd = linearController.kD;

  chassis.moveTo(0,20,-90, false, 9, 12, lkp, lki, lkd, akp, aki, akd, .35, .6, 2);
  wait(1500, msec);
  chassis.moveTo(20,20,90, false, 9, 12, lkp, lki, lkd, akp, aki, akd, .35, .6, 2);
  wait(1500, msec);
  chassis.moveTo(0,0,0, false, 9, 12, lkp, lki, lkd, akp, aki, akd, .35, .6, 2);
  wait(100000, msec);
}

void usercontrol(void)
{
  chassis.stateMachineOff();
  printf("Entered Driver\n");
  chassis.stateMachineOff();

  while (1)
  {
    chassis.tank(Controller.Axis3.position(), Controller.Axis2.position(), 100);
    wait(10.0, msec);
  }
}

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
