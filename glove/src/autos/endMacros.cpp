#include "autos.h"
#include "vex.h"
using namespace tntnlib;


void ElimShootMacro()
{
  intakeVolts = -12;
  antijam = true;
  while (getRunTime() < 44.95)
  {
    wait(10, vex::msec);
  }
  chassis.stopAuto();
  intakeVolts = 0;
  delay(100000);
}

void poleTouchMacro()
{
  intakeVolts = -12;
  antijam = true;
  while (getRunTime() < 39.25)
  {
    wait(10, vex::msec);
  }

  // back out of goal
  chassis.moveTo(109.5, 98, true, 12, 12, lkp * .8, lki, lkd * 1.4, akp * 1.3, aki, akd * 1.6, 12, 5);

  // clear balls in the way
  chassis.turnToHeading(170, false, 12, akp, aki, akd, 5);
  intakeVolts = (4);
  chassis.pid(50, 170, false, 11, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
  delay(750);

  chassis.moveTo(107, 93, true, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.6, 12, 3);
  chassis.turnToHeading(-19, false, 12, akp, aki, akd, 5);
  Path touchPolePath(107, 89, 108.6, 85.2, 111.7, 84.4, 110, 71, 100);
  chassis.follow(touchPolePath, true, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 12, 15);
  intakeVolts = -6;
  delay(400);
  chassis.turnToHeading(30, false, 12, akp * 1.5, aki, akd, 5);
  delay(500);
  chassis.stopAuto();
  intakeVolts = 0;
}