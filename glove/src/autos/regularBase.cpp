#include "autos.h"
#include "vex.h"
using namespace tntnlib;

void baseMatchAuto()
{
  chassis.startAuto(131, 54, 0);
  chassis.breakOutTimeMs = 3500;
  Path path1(131, 59, 133.3, 104.5, 129.6, 112.0, 122, 118.3, 100);
  chassis.setOffset(0, 0);
  chassis.follow(path1, false, 12, 12, lkp * .9, lki, lkd, akp, aki, akd * 1.4, 12, 16, 20);
  right_wing.set(true);
  left_wing.set(true);
  Pose fake(path1.x3, path1.y3, 0);
  do
  {
    wait(10, vex::msec);
  } while (fabs(chassis.getPose().distance(fake)) > 10);
  delay(700);
  intakeVolts = (-12);
  chassis.pid(5, -38, false, 5, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
  delay(700);
  chassis.turnToHeading(-95, false, 12, akp, aki, akd, 5);
  right_wing.set(false);
  left_wing.set(false);
  chassis.turnToHeading(-45, false, 12, akp, aki, akd, 5);
  chassis.pid(-5, -45, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 2);

  chassis.turnToHeadingUnbounded(110, false, 12, akp, aki, akd, 6);
  chassis.setOffset(0, 0);
  chassis.moveTo(115, 130, true, 10, 12, lkp, lki, lkd, akp, aki, akd * 1.25, .4, 12);
  chassis.moveTo(90, 127, true, 12, 12, lkp * 1.4, lki, lkd, akp, aki, akd * 1.25, 12, 20); // shove 1
  delay(400);
  chassis.pid(6, 90, false, 12, 12, lkp, lki, lkd, akp, aki, akd, .2, 0);
  delay(500);
  chassis.pid(-50, 90, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
  delay(650);

  // contest ball in neutral and shove it
  chassis.setOffset(0, 8);
  intakeVolts = 12;
  chassis.breakOutTimeMs = 10000;
  Path path2(104.5, 126.1, 148.3, 133.1, 142.0, 104.8, 71.4, 95.9, 100);
  chassis.follow(path2, false, 12, 12, lkp * .6, lki, lkd, akp, aki, akd * 1.25, .4, 22, 6);
  chassis.breakOutTimeMs = 5000;
  chassis.setOffset(0, 8);
  chassis.turnToHeading(0, false, 12, akp, aki, akd, 100);
  left_wing.set(true);
  chassis.turnToHeading(0, false, 12, akp, aki, akd, 6);
  right_wing.set(true);
  intakeVolts = -12;
  chassis.pid(23, 0, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
  delay(900);
  wings(0, 0);

  chassis.moveTo(94, 97, true, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 5); // back out of goal
  chassis.turnToPose(96, 74, false, 12, akp, aki, akd * 1.6, 10);
  intakeVolts = (4);

  chassis.moveTo(99, 77, false, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 5); // grab ball 1
  chassis.pid(-15, 180, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 5);
  wings(1, 0);
  chassis.moveTo(80.6, 116, false, 12, 12, lkp, lki, lkd, akp * 1.0, aki, akd * 1.8, 12, 10); // go into goal
  intakeVolts = (-12);

  chassis.pid(6, -20, false, 3, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
  delay(200);
  chassis.SwingOnRightToHeading(10, false, 12, akp * 3, aki, akd, 10);
  delay(200);
  wings(0, 0);
  chassis.SwingOnRightToHeading(-20, false, 12, akp * 1.6, aki, akd, 0);
  delay(200);
  chassis.pid(6, -20, false, 3, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);

  // stopAuto();
  //  get awp pole touch
  endTimer();
}