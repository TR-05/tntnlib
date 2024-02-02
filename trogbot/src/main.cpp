#include "vex.h"
#include "../tntnlibrary/include/api.h"
#include "autos.h"
#include "vision.h"

#include <iostream>
using namespace tntnlib;
vex::competition Competition;
vex::brain Brain;

vex::vision::signature SIG_1 (1, 4241, 4645, 4444, -1031, -683, -856, 2.800, 0);
vex::vision vision1(vex::PORT13, 18, SIG_1);

/* tntnlib robot Config */
MotorGroup leftMotors(vex::gearSetting::ratio6_1, 360, -8, -10);
MotorGroup rightMotors(vex::gearSetting::ratio6_1, 360, 18, 19);
TrackingWheel horizontal(Brain.ThreeWirePort.G, Omniwheel::NEW_275, 0.002292, 1);
TrackingWheel vertical(Brain.ThreeWirePort.E, Omniwheel::NEW_275, -0.253611, 1);
Gyro imu(1, 1.010357);
/* chassis and controllers (DO NOT CHANGE NAMES) */
ControllerSettings linearSettings(.6, 0, 3.5, 0, 0, 12);
ControllerSettings angularSettings(.25, 0.01, 2.0, 2, 15, 12);
Drivetrain drivetrain(&leftMotors, &rightMotors, 10.0, Omniwheel::OLD_325, 360, 8);
OdomSensors sensors(&vertical, nullptr, &horizontal, nullptr, &imu);
Chassis chassis(drivetrain, linearSettings, angularSettings, sensors);
// MotorGroup intake(vex::gearSetting::ratio6_1, 360, -8, -10);

/* End of tntnlib Robot Config */

/* data logger idk where to put :/ */

int logger()
{
  while (true)
  {
    Pose current(chassis.getPose(false));
    // printf("SX: %.2f, SR: %.2f, IMU: %.2f ", chassis.sensors.horizontal1 != nullptr ? chassis.sensors.horizontal1->getDistance() : 0, chassis.sensors.vertical1 != nullptr ? chassis.sensors.vertical1->getDistance() : 0, chassis.sensors.gyro != nullptr ? chassis.sensors.gyro->rotation() : 0);
    // printf("  X: %.2f,  Y: %.2f,  H: %.2f   T: %.2f ET:%.2f\n", current.x, current.y, current.theta, getTime(), totalTime / 1000.0);
    printf("%.2f,%.2f,%.2f,%.2f", current.x, current.y, leftMotors.getRPM(), getTime());
    std::cout << std::flush;
    // Brain.Screen.print("X:%6.2f, Y:%6.2f, H:%6.2f", current.x, current.y, current.theta);
    vex::wait(50, vex::msec);
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
    updateButtons();
    float pow = 0; //visionPower();
    if (Controller.ButtonR1.pressing())
    { 
      pow = 0;
    }
    chassis.tank(Controller.Axis3.position() * .12 + pow, Controller.Axis2.position() * .12 - pow, 0); // tank (the best drive style)
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
