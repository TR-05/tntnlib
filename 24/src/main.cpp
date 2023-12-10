#include "vex.h"
#include "../tntnlibrary/include/api.h"
#include "autos.h"
#include <iostream>
using namespace vex;
competition Competition;
brain Brain;

/* tntnlib robot Config */

/* drive motors */
motor ls_1 = motor(PORT7, ratio6_1, false);
motor ls_2 = motor(PORT8, ratio6_1, true);
motor ls_3 = motor(PORT9, ratio6_1, true);
motor ls_4 = motor(PORT10, ratio6_1, false);

motor rs_1 = motor(PORT1, ratio6_1, true);
motor rs_2 = motor(PORT2, ratio6_1, false);
motor rs_3 = motor(PORT3, ratio6_1, false);
motor rs_4 = motor(PORT4, ratio6_1, true);
//, ls_mid, ls_back, ls_top
motor_group leftMotors(ls_1, ls_2, ls_3, ls_4);
motor_group rightMotors(rs_1, rs_2, rs_3, rs_4);
tntnlib::MotorGroup leftMotors2(ratio6_1, 600, 7, 8, 9, 10);
/* tracking wheels and gyro */
tntnlib::TrackingWheel horizontal(Brain.ThreeWirePort.G, tntnlib::Omniwheel::NEW_275, 0.002292, 1);
// tntnlib::TrackingWheel vertical(Brain.ThreeWirePort.E, tntnlib::Omniwheel::NEW_275, -0.253611, 1);
tntnlib::TrackingWheel vertical(&leftMotors, tntnlib::Omniwheel::NEW_4, -8, 600, 300);
tntnlib::Gyro imu(PORT15, 1.010357);

/* chassis and controllers */
tntnlib::ControllerSettings tntnlib::linearSettings(.6, 0, 3.5, 0, 0, 12);
tntnlib::ControllerSettings tntnlib::angularSettings(.25, 0.01, 2.0, 10, 2, 12);
tntnlib::Drivetrain tntnlib::drivetrain(&leftMotors, &rightMotors, 10.0, tntnlib::Omniwheel::OLD_325, 360, 8);
tntnlib::OdomSensors tntnlib::sensors(&vertical, nullptr, nullptr, nullptr, nullptr);
/* End of tntnlib Robot Config */

tntnlib::Flywheel flywheel(ratio6_1, 3600, 11, 0, 0.0, 2, -12, 11, 20, -19);
tntnlib::Intake intake(ratio6_1, 600, -13, 17);

digital_out left_intake_piston = digital_out(Brain.ThreeWirePort.A);
digital_out right_intake_piston = digital_out(Brain.ThreeWirePort.B);

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
    wait(50, msec);
  }
  return 0;
}

/* runs when program first starts */
void pre_auton()
{
  printf("Entered pre_auton\n");
  chassis.initialize(true, 0, 0, 0);
  task log(logger);
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
  wait(200, msec);
  left_intake_piston.set(1);
  right_intake_piston.set(1);
}

void loadMacro(int times)
{
  for (int i = 0; i < times; i++) 
  {
    singleLoadMacro();
    wait(1000, msec);
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
    intake.driver(Controller.ButtonR1.pressing(), Controller.ButtonR2.pressing(), 12, -12);

    // chassis.tank(Controller.Axis3.position(), Controller.Axis2.position(), 100); // tank (the best drive style)
    //  chassis.arcade(Controller.Axis3.position(), Controller.Axis4.position(), 0); //single stick arcade
    chassis.arcade(Controller.Axis3.position(), Controller.Axis1.position(), 0); // split arcade
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
