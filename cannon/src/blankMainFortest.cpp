/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       trevor                                                    */
/*    Created:      2/9/2024, 4:09:20 PM                                      */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
#include <iostream>
using namespace vex;
brain Brain;
// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void)
{

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void)
{
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/
// MotorGroup flywheel(vex::gearSetting::ratio6_1, 3600, -12, 11, 20, -19);
motor fl1(PORT12, ratio6_1, true);
motor fl2(PORT11, ratio6_1, false);
motor fl3(PORT20, ratio6_1, false);
motor fl4(PORT19, ratio6_1, true);
motor i1(PORT13, ratio6_1, true);
motor i2(PORT17, ratio6_1, false);

controller Con;
float RPM = 0;

float getRPM()
{
  RPM = (fl1.velocity(pct) + fl2.velocity(pct) + fl3.velocity(pct) + fl4.velocity(pct)) * 36 / 4;
  return RPM;
}
int sgn(float x)
{
  if (x < 0)
    return -1;
  else
    return 1;
}

float bangBangMargin = 2.0;
float integral = 0;
float lastError = 0;
float kV = 11.75, kP = 0, kI = .075, kAcc = 1, kDec = 1;
float getPower(float rpm)
{
  float error = (rpm - getRPM()) / 3600;
  if (error > bangBangMargin)
  {
    integral = 0;
    return 12;
  }
  else
  {
    integral += error;
    if (sgn(error) != sgn(lastError) && sgn(error) == 1)
    {
      integral /= 2;
    }
    lastError = error;
    float power = (kV * rpm / 3600.0) + kP * error + kI * integral;
    if (sgn(power) == sgn(getRPM()))
      power *= kAcc;
    else
      power *= kDec;
    printf("\n\ncurRpm: %.2f power: %.2f integral: %.2f kI: %.2f rpm: %.3f\n", rpm, power, integral, kI, getRPM());
    return power;
  }
}

void usercontrol(void)
{
  // User control code here, inside the loop
  while (1)
  {
    if (Con.ButtonR1.pressing())
    {
      float pow = getPower(3600);
      fl1.spin(fwd, pow, volt);
      fl2.spin(fwd, pow, volt);
      fl3.spin(fwd, pow, volt);
      fl4.spin(fwd, pow, volt);
    }
    else
    {
      fl1.stop();
      fl2.stop();
      fl3.stop();
      fl4.stop();
    }

    if (Con.ButtonL1.pressing())
    {
      i1.spin(fwd, 12, volt);
      i2.spin(fwd, 12, volt);
    }
    else
    {
      i1.stop();
      i2.stop();
    }

    float volt = (fl1.voltage() + fl2.voltage() + fl3.voltage() + fl4.voltage()) / 4;

    float endTime = Brain.timer(vex::msec) / 1000;
    printf("%.2f,%.2f,%.2f,%.2f\n", endTime, RPM, volt, 0.0);
    std::cout << std::flush;

    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main()
{
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true)
  {
    wait(100, msec);
  }
}
