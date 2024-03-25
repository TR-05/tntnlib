#include "autos.h"
#include "vex.h"
using namespace tntnlib;

void programming_skills()
{
  chassis.startAuto(131, 54, 0);
  chassis.breakOutTimeMs = 3500;

  // curve around to side of goal with preload, 1 ball, and matchload ball
  Path curveInPath(131, 59, 135.9, 111.2, 138.2, 139.1, 98, 126, 100);
  chassis.setOffset(0, 0);
  chassis.follow(curveInPath, false, 12, 12, lkp * .9, lki, lkd, akp, aki, akd, 12, 18, 0);
  // black magic wing timing (don't change)
  delay(1050);
  wings(1, 1);
  delay(550);
  wings(0, 0);
  intakeVolts = -12;
  chassis.LineWait(curveInPath.x3, curveInPath.y3, 10, 3000);
  delay(250);

  // line up for passing play
  Path lineUpPath(98.2, 131.9, 142.8, 116.6, 89.9, 90.4, 58.2, 101.4, 100);
  chassis.follow(lineUpPath, true, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 12, 3);
  chassis.turnToHeading(19, false, 12, akp, aki, akd * 1.2, 3);
  chassis.pid(10, 19, false, 8, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
  wings(0, 1);
  delay(750);
  chassis.pid(4, 19, false, 2, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);

  // wait till left side shooting is done
  antijam = true;
  while (getRunTime() < 23)
  {
    delay(10);
  }
  antijam = false;

  // shove sequence
  wings(0, 0);
  // back out
  chassis.moveTo(67, 85, true, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.5, 12, 5);
  wings(1, 1);
  // go in
  chassis.moveTo(71, 118, false, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 0);
  delay(1000);
  wings(0, 0);

  // line up for right side
  chassis.moveTo(89, 84, true, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 3);
  chassis.turnToPose(84, 105, false, 12, akp, aki, akd, 3);
  chassis.moveTo(84, 105, false, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 7);
  chassis.turnToHeading(-19, false, 12, akp, aki, akd, 3);
  chassis.pid(10, -19, false, 4, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
  wings(1, 0);
  delay(750);
  chassis.pid(4, -19, false, 3, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
  antijam = true;
  while (getRunTime() < 54.9)
  {
    delay(10);
  }
  antijam = false;

  // double shove for funny ig
  // shove 1
  wings(0, 0);
  intakeVolts = 0;
  chassis.moveTo(67, 85, true, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.5, 12, 5);
  wings(1, 1);
  chassis.moveTo(71, 118, false, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.5, 12, 0);
  intakeVolts = -12;
  delay(1000);

  // back out
  chassis.pid(-20, 0, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
  delay(1000);
  chassis.moveTo(71, 118, false, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.5, 12, 0);
  delay(1000);

  // back out
  chassis.pid(-15, 0, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
  delay(1000);
  wings(0, 0);

  /* none working hang setup

  chassis.SwingOnRightToHeading(15, 0, 12, akp * 1.6, aki, akd, 0);
  delay(400);
  right_wing.set(0);
  left_wing.set(0);
  Path path3(84, 105, 88.7, 98.2, 100.5, 84, 110.75, 91, 100);
  chassis.follow(path3, true, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 12, 3);

  //  chassis.moveTo(108.5, 88.5, true, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 5);
  chassis.turnToHeading(174, false, 12, akp, aki, akd, 20);
  delay(200);
  intakeVolts = (12);
  chassis.pid(40, 174, false, 10, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
  delay(700);
  chassis.autoTankVolts(7.5, 8);
  delay(1200);
  chassis.autoTankVolts(0, 0);
  delay(200);
  hang.set(1);
  while (getRunTime() < 59.85)
  {
    delay(10);
  }
  hang.set(0);
  */

  chassis.stopAuto();
  intakeVolts = 0;
  delay(100000);
}