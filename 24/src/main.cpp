#include "vex.h"
#include "../tntnlibrary/include/api.h"
#include "autos.h"
#include <iostream>
using namespace tntnlib;
vex::competition Competition;
vex::brain Brain;

/* tntnlib robot Config */
MotorGroup leftMotors(vex::gearSetting::ratio6_1, 300, 7, -8, -9, 10);
MotorGroup rightMotors(vex::gearSetting::ratio6_1, 300, -1, 2, 3, -4);
TrackingWheel horizontal(Brain.ThreeWirePort.G, tntnlib::Omniwheel::NEW_275, 0.002292, 1);
TrackingWheel vertical(&leftMotors, tntnlib::Omniwheel::NEW_4, -8, 600, 300);
Gyro imu(15, 1.010357);
/* chassis and controllers (DO NOT CHANGE NAMES) */
ControllerSettings tntnlib::linearSettings(.6, 0, 3.5, 0, 0, 12);
ControllerSettings tntnlib::angularSettings(.25, 0.01, 2.0, 10, 2, 12);
Drivetrain tntnlib::drivetrain(&leftMotors, &rightMotors, 10.0, tntnlib::Omniwheel::OLD_325, 360, 8);
OdomSensors tntnlib::sensors(&vertical, nullptr, nullptr, nullptr, nullptr);
Flywheel flywheel(vex::gearSetting::ratio6_1, 3600, 11, 0, 0.0, 2, -12, 11, 20, -19);
MotorGroup intake(vex::gearSetting::ratio6_1, 600, -13, 17);
/* End of tntnlib Robot Config */

vex::digital_out left_intake_piston(Brain.ThreeWirePort.A);
vex::digital_out right_intake_piston(Brain.ThreeWirePort.B);

/* data logger idk where to put :/ */
int logger()
{
  while (true)
  {
    tntnlib::Pose current(chassis.getPose(false));
    printf("F: %.2f,    SX: %.2f, SR: %.2f, IMU: %.2f ", flywheel.currentRPM, tntnlib::sensors.horizontal1 != nullptr ? tntnlib::sensors.horizontal1->getDistance() : 0, tntnlib::sensors.vertical1 != nullptr ? tntnlib::sensors.vertical1->getDistance() : 0, tntnlib::sensors.gyro != nullptr ? tntnlib::sensors.gyro->rotation() : 0);
    printf("X: %.2f,Y: %.2f,H: %.2f BH: %.2f\n", current.x, current.y, current.theta, fmod(current.theta, 360));
    Brain.Screen.clearLine();
    Brain.Screen.print("X:%6.2f, Y:%6.2f, H:%6.2f", current.x, current.y, current.theta);
    vex::wait(50, vex::msec);
  }
  return 0;
}

/* runs when program first starts */
void pre_auton()
{
  printf("Entered pre_auton\n");
  chassis.initialize(true, 0, 0, 0);
  vex::task log(logger);
}

/* runs on comp switch autonomous */
void autonomous()
{
  programming_skills();
}




void singleLoadMacro()
{
  intake.spinVolts(12);
  left_intake_piston.set(0);
  right_intake_piston.set(0);
  vex::wait(200, vex::msec);
  left_intake_piston.set(1);
  right_intake_piston.set(1);
}

void loadMacro(int times)
{
  for (int i = 0; i < times; i++) 
  {
    singleLoadMacro();
    vex::wait(1000, vex::msec);
  }
}

/* runs on comp switch driver */
void usercontrol()
{
  printf("Entered Driver\n");
  chassis.stateMachineOff();

  bool flywheelOn = false;
  // User control code here, inside the loop
  float rpm = 0;
  bool pistonState = false;
  bool lastIntakeMacro = false;
  while (1)
  {

    if (Controller.ButtonA.pressing())
    {
      rpm = 3600;
      flywheelOn = true;
    }

    if (Controller.ButtonB.pressing())
    {
      rpm = 2600;
      flywheelOn = true;
    }
    if (Controller.ButtonL2.pressing())
    {
      flywheelOn = false;
    }

    if (flywheelOn)
    {
      flywheel.spinRPM(rpm);
    }
    else
    {
      flywheel.spinVolts(0);
    }
    if (Controller.ButtonL1.pressing() && !pistonState)
    {
      left_intake_piston.set(!left_intake_piston);
      right_intake_piston.set(!left_intake_piston);
    }
    pistonState = Controller.ButtonL1.pressing();

    if (Controller.ButtonRight.pressing() && !lastIntakeMacro)
    {
      loadMacro(30);
    }
    lastIntakeMacro = Controller.ButtonRight.pressing();
    intake.driverTwoButton(Controller.ButtonR1.pressing(), Controller.ButtonR2.pressing(), 12, -12);

    // chassis.tank(Controller.Axis3.position(), Controller.Axis2.position(), 100); // tank (the best drive style)
    //  chassis.arcade(Controller.Axis3.position(), Controller.Axis4.position(), 0); //single stick arcade
    chassis.arcade(Controller.Axis3.position(), Controller.Axis1.position(), 0); // split arcade
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
