#include "vex.h"
#include "../tntnlibrary/include/api.h"
#include "autos.h"
#include <iostream>
using namespace vex;
competition Competition;
brain Brain;

/* tntnlib robot Config */

/* drive motors */
motor ls_front = motor(PORT7, ratio6_1, false);
motor ls_mid = motor(PORT6, ratio6_1, true);
motor ls_back = motor(PORT8, ratio6_1, true);
motor ls_top = motor(PORT5, ratio6_1, false);

motor rs_front = motor(PORT3, ratio6_1, true);
motor rs_mid = motor(PORT13, ratio6_1, false);
motor rs_back = motor(PORT14, ratio6_1, false);
motor rs_top = motor(PORT15, ratio6_1, true);
//, ls_mid, ls_back, ls_top
motor_group leftMotors = motor_group(ls_front, ls_mid, ls_back, ls_top);
motor_group rightMotors = motor_group(rs_front, rs_mid, rs_back, rs_top);

/* tracking wheels and gyro */
tntnlib::TrackingWheel horizontal(Brain.ThreeWirePort.G, tntnlib::Omniwheel::NEW_275, 0.002292, 1);
// tntnlib::TrackingWheel vertical(Brain.ThreeWirePort.E, tntnlib::Omniwheel::NEW_275, -0.253611, 1);
tntnlib::TrackingWheel vertical(&leftMotors, tntnlib::Omniwheel::NEW_4, -8, 600, 300);
tntnlib::Gyro imu(1, 1.010357);

/* chassis and controllers */
tntnlib::ControllerSettings tntnlib::linearSettings(.6, 0, 3.5, 0, 0, 12);
tntnlib::ControllerSettings tntnlib::angularSettings(.25, 0.01, 2.0, 10, 2, 12);
tntnlib::Drivetrain tntnlib::drivetrain(&leftMotors, &rightMotors, 10.0, tntnlib::Omniwheel::OLD_325, 360, 8);
tntnlib::OdomSensors tntnlib::sensors(&vertical, nullptr, nullptr, nullptr, nullptr);
/* End of tntnlib Robot Config */

motor DFWl1 = motor(PORT11, ratio6_1, true);
motor DFWl2 = motor(PORT12, ratio6_1, false);
motor DFWr1 = motor(PORT20, ratio6_1, false);
motor DFWr2 = motor(PORT19, ratio6_1, true);

std::vector<vex::motor> flywheelMotors = {DFWl1, DFWl2, DFWr1, DFWr2};
tntnlib::Flywheel flywheel(&flywheelMotors, 600, 3600, 11, 0, 0.0, 2);

motor left_intake = motor(PORT10, ratio6_1, false);
motor right_intake = motor(PORT17, ratio6_1, true);

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
      left_intake.spin(fwd, 12, volt);
      right_intake.spin(fwd, 12, volt);
      left_intake_piston.set(0);
      right_intake_piston.set(0);
      wait(200, msec);
      left_intake_piston.set(1);
      right_intake_piston.set(1);
    }
    lastIntakeMacro = Controller.ButtonRight.pressing();
    float intakePower = 12 * (Controller.ButtonR1.pressing() - Controller.ButtonR2.pressing());
    left_intake.spin(fwd, intakePower, volt);
    right_intake.spin(fwd, intakePower, volt);

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
