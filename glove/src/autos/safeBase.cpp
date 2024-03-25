#include "autos.h"
#include "vex.h"
using namespace tntnlib;


void safeBaseMatchAuto()
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

  // grab 1 safe ball from far side
  intakeVolts = 0;
  chassis.breakOutTimeMs = 10000;
  chassis.autoTankVolts(0, 0);
  delay(5000);
  Path sillyCurve1(104.5, 126.1, 131.6, 114.6, 136.8, 105.7, 106.3, 100.5, 100);
  Path sillyCurve2(105.4, 100.8, 103.1, 95.6, 100.5, 92.2, 99.4, 64.8, 100);
  Path sillyCurve = sillyCurve1 + sillyCurve2;
  intakeVolts = 12;
  chassis.follow(sillyCurve, false, 12, 12, lkp * .6, lki, lkd, akp, aki, akd * 1.25, .4, 22, 30);
  delay(1000);
  chassis.breakOutTimeMs = 5000;

  // turn around and score in goal
  // chassis.pid(-15, 180, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 5);
  chassis.moveTo(81.5, 116, false, 6, 12, lkp * .8, lki, lkd, akp * 1.0, aki, akd * 1.8, 12, 18); // go into goal
  intakeVolts = -12;
  delay(500);

  // line up for catching
  chassis.pid(6, -15, false, 3, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
  delay(200);
  chassis.SwingOnRightToHeading(10, false, 12, akp * 3, aki, akd, 10);
  delay(200);
  wings(0, 0);
  chassis.SwingOnRightToHeading(-15, false, 12, akp * 1.6, aki, akd, 0);
  delay(200);
  chassis.pid(6, -15, false, 3, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
  endTimer();
}