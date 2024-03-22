#include "autos.h"
#include "vex.h"
using namespace tntnlib;

void baseMatchAuto();
/* Example auto move functions
 chassis.tuneOffsets(3600, akp, aki, akd, 6, 2); //Tunes odom + imu constants: DISABLE TERMINAL PRINT then jig bot to a tile, run auto, once bot stops moving tap and hold brain screen
 chassis.pid(24, 60, 60, false, 12, 12, lkp, lki, lkd, akp, aki, akd, .4, 2); //Moves forward 24 inches looking at xy point (60,60)
 chassis.pid(-24, 0, true, 12, 12, lkp, lki, lkd, akp, aki, akd, .4, 2); //Moves backward 24 inches holding heading 0,0
 chassis.SwingOnLeftToHeading(-45, 0, 12, akp*1.6, aki, akd, 1);
 chassis.moveTo(0, 20, false, 12, 12, lkp, lki, lkd, akp*1.3, aki, akd * 1.25, 12, 2);
 chassis.boomerangTo(0, 20, -180, false, 12, 12, lkp, lki, lkd, akp, aki, akd, .2, .5, 12, 2);
 chassis.turnToHeading(0, false, 12, akp, aki, akd, 1);
*/

void wings(bool left, bool right)
{
  left_wing.set(left);
  right_wing.set(right);
}

void touchAWP()
{
  while (getRunTime() < 39.25)
  {
    wait(10, vex::msec);
  }

  //back out of goal
  chassis.moveTo(109.5, 98, true, 12, 12, lkp*.8, lki, lkd*1.4, akp * 1.3, aki, akd * 1.6, 12, 5);

  //clear balls in the way
  chassis.turnToHeading(170, false, 12, akp, aki, akd, 5);
  intakeVolts = (4);
  chassis.pid(50, 170, false, 11, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
  delay(750);

  chassis.moveTo(107, 93, true, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.6, 12, 3);
  chassis.turnToHeading(-19, false, 12, akp, aki, akd, 5);
  Path touchPolePath(107,89,  108.6,85.2,  111.7,84.4,  110,71,  100);
  chassis.follow(touchPolePath, true, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 12, 15);
  intakeVolts = -6;
  delay(400);
  chassis.turnToHeading(30, false, 12, akp*1.5, aki, akd, 5);
  delay(500);
  chassis.stopAuto();
  intakeVolts = 0;
}

void programming_skills()
{
  chassis.startAuto(131, 54, 0);
  chassis.breakOutTimeMs = 3500;

  //curve around to side of goal with preload, 1 ball, and matchload ball
  Path curveInPath(131, 59, 135.9, 111.2, 138.2, 139.1, 98, 126, 100);
  chassis.setOffset(0, 0);
  chassis.follow(curveInPath, false, 12, 12, lkp * .9, lki, lkd, akp, aki, akd, 12, 18, 0);
  //black magic wing timing (don't change)
  delay(1050);
  wings(1,1);
  delay(550);
  wings(0,0);
  intakeVolts = -12;
  chassis.LineWait(curveInPath.x3, curveInPath.y3, 10, 3000);
  delay(250);

  //line up for passing play  
  Path lineUpPath(98.2, 131.9, 142.8, 116.6, 89.9, 90.4, 58.2, 101.4, 100);
  chassis.follow(lineUpPath, true, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 12, 3);
  chassis.turnToHeading(19, false, 12, akp, aki, akd*1.2, 3);
  chassis.pid(10, 19, false, 8, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
  wings(0, 1);
  delay(750);
  chassis.pid(4, 19, false, 2, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);

  //wait till left side shooting is done
  antijam = true;
  while (getRunTime() < 23)
  {
    delay(10);
  }
  antijam = false;

  // shove sequence
  wings(0,0);
  //back out
  chassis.moveTo(67, 85, true, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.5, 12, 5);
  wings(1,1);
  //go in
  chassis.moveTo(71, 118, false, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 0);
  delay(1000);
  wings(0,0);

  //line up for right side
  chassis.moveTo(89, 84, true, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 3);
  chassis.turnToPose(84, 105, false, 12, akp, aki, akd, 3);
  chassis.moveTo(84, 105, false, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 7);
  chassis.turnToHeading(-19, false, 12, akp, aki, akd, 3);
  chassis.pid(10, -19, false, 4, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
  wings(1,0);
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
  wings(0,0);
  intakeVolts = 0;
  chassis.moveTo(67, 85, true, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.5, 12, 5);
  wings(1,1);
  chassis.moveTo(71, 118, false, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.5, 12, 0);
  intakeVolts = -12;
  delay(1000);
  wings(0,0);

  // back out
  chassis.pid(-15, 0, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
  delay(1000);

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

void ElimAwp()
{
  baseMatchAuto();
  while (getRunTime() < 45.5)
  {
    wait(10, vex::msec);
  }
  chassis.stopAuto();
  intakeVolts = 0;
  delay(100000);
}

void awp()
{
  baseMatchAuto();
  touchAWP();
}

void baseMatchAuto()
{
  chassis.startAuto(131, 54, 0);
  chassis.breakOutTimeMs = 3500;
  Path path1(131, 59, 133.3, 104.5, 129.6, 112.0, 122, 118.3, 100);
  chassis.setOffset(0, 0);
  chassis.follow(path1, false, 12, 12, lkp * .9, lki, lkd, akp, aki, akd*1.4, 12, 16, 20);
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
  Path path2(104.5,126.1,  148.3,133.1,  142.0,104.8,  71.4,95.9,  100);
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
  wings(0,0);

  chassis.moveTo(94, 97, true, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 5); // back out of goal
  chassis.turnToPose(96, 74, false, 12, akp, aki, akd * 1.6, 10);
  intakeVolts = (4);
  
  chassis.moveTo(99, 77, false, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 5); // grab ball 1
  chassis.pid(-15, 180, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 5);
  wings(1,0);
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

void safeBaseMatchAuto()
{

  chassis.startAuto(131, 54, 0);
  chassis.breakOutTimeMs = 3500;
  Path path1(131, 59, 133.3, 104.5, 129.6, 112.0, 122, 118.3, 100);
  chassis.setOffset(0, 0);
  chassis.follow(path1, false, 12, 12, lkp * .9, lki, lkd, akp, aki, akd, 12, 16, 20);
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
  chassis.moveTo(115, 128, true, 10, 12, lkp, lki, lkd, akp, aki, akd * 1.25, .4, 9);
  chassis.moveTo(90, 126.5, true, 12, 12, lkp * 1.4, lki, lkd, akp, aki, akd * 1.25, 12, 20); // shove 1
  delay(400);
  chassis.pid(6, 90, false, 12, 12, lkp, lki, lkd, akp, aki, akd, .2, 0);
  delay(500);
  chassis.pid(-50, 90, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
  delay(900);

  // contest ball in neutral and shove it
  chassis.setOffset(0, 8);
  intakeVolts = (12);
  chassis.breakOutTimeMs = 10000;

  Path path2(104.5, 126.1, 154.9, 128.4, 108.0, 99.9, 84.4, 106.0, 100);
  chassis.follow(path2, false, 12, 12, lkp * .6, lki, lkd, akp, aki, akd * 1.25, .4, 22, 6);
  chassis.turnToPose(80.6, 116, false, 12, akp, aki, akd, 5);
  chassis.moveTo(80.6, 116, false, 12, 12, lkp, lki, lkd, akp * 1.0, aki, akd * 1.8, 12, 10); // go into goal
  intakeVolts = (-12);

  chassis.turnToHeading(-20, false, 12, akp, aki, akd, 5);
  chassis.pid(7, -20, false, 3, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);

  // stopAuto();
  //  get awp pole touch
  endTimer();
}

void safeAWP()
{
  safeBaseMatchAuto();
  touchAWP();
}

void safeElim()
{
  safeBaseMatchAuto();
  while (getRunTime() < 45.5)
  {
    wait(10, vex::msec);
  }
  chassis.stopAuto();
  intakeVolts = 0;
  delay(100000);
  return;
}