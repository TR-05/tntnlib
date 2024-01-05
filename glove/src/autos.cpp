#include "autos.h"
#include "vex.h"
using namespace tntnlib;

void delay(float ms)
{
  vex::wait(ms, vex::msec);
}

float startTime{0}, endTime{0}, totalTime{0};

/* Example auto move functions
 chassis.tuneOffsets(3600, akp, aki, akd, 6, 2); //Tunes odom + imu constants: DISABLE TERMINAL PRINT then jig bot to a tile, run auto, once bot stops moving tap and hold brain screen
 chassis.pid(24, 60, 60, false, 12, 12, lkp, lki, lkd, akp, aki, akd, .4, 2); //Moves forward 24 inches looking at xy point (60,60)
 chassis.pid(-24, 0, true, 12, 12, lkp, lki, lkd, akp, aki, akd, .4, 2); //Moves backward 24 inches holding heading 0,0
 chassis.SwingOnLeftToHeading(-45, 0, 12, akp*1.6, aki, akd, 1);
 chassis.moveTo(0, 20, false, 12, 12, lkp, lki, lkd, akp*1.3, aki, akd * 1.25, 12, 2);
 chassis.boomerangTo(0, 20, -180, false, 12, 12, lkp, lki, lkd, akp, aki, akd, .2, .5, 12, 2);
 chassis.turnToHeading(0, false, 12, akp, aki, akd, 1);
*/
float akp = chassis.angularSettings.kP;
float aki = chassis.angularSettings.kI;
float akd = chassis.angularSettings.kD;
float lkp = chassis.linearSettings.kP;
float lki = chassis.linearSettings.kI;
float lkd = chassis.linearSettings.kD;

void printTime()
{
  endTime = Brain.timer(vex::msec);
  totalTime = endTime - startTime;
  printf("Time: %.2f\n", totalTime / 1000.0);
}

float getTime()
{
  endTime = Brain.timer(vex::msec);
  return (endTime - startTime) / 1000.0;
}
void startAuto(float x, float y, float theta)
{
  printf("Entered Auto\n");
  startTime = Brain.timer(vex::msec);
  chassis.initialize(false, x, y, theta);
  chassis.setOffset(0, 0);
  chassis.stateMachineOn();
  akp = chassis.angularSettings.kP;
  aki = chassis.angularSettings.kI;
  akd = chassis.angularSettings.kD;
  lkp = chassis.linearSettings.kP;
  lki = chassis.linearSettings.kI;
  lkd = chassis.linearSettings.kD;
}

void stopAuto()
{
  chassis.stateMachineOff();
  delay(20);
  chassis.tank(0, 0);
  intake.stop(vex::brakeType::coast);
  printTime();
}
void programming_skills()
{
  startAuto(131.75, 54, 0);
  Path path1(131, 59, 129.6, 123.6, 132.8, 136.4, 98.2, 136.5, 100);
  chassis.setOffset(0, 8);
  chassis.follow(path1, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 12, 10);
  intake.spinVolts(-12);
  delay(500);
  chassis.setOffset(0, 0);
  chassis.pid(-14, -90, false, 6, 12, lkp, lki, lkd, akp, aki, akd, .4, 2);
  chassis.turnToHeading(98, false, 12, akp, aki, akd, 1);
  chassis.pid(-50, 98, false, 3, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
  delay(500);
  Path path2(98.2, 131.9, 142.8, 116.6, 72.3, 69.7, 59.0, 104, 100);
  chassis.follow(path2, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 12, 3);
  delay(500);
  chassis.turnToHeading(15, false, 12, akp, aki, akd, 1);
  chassis.pid(4.5, 15, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 3);
  delay(500);
  chassis.turnToHeading(15, false, 12, akp, aki, akd, 1);
  delay(1000);
 chassis.moveTo(70.9, 91, true, 12, 12, lkp, lki, lkd, akp*1.3, aki, akd * 1.25, 12, 2);
  Path path3(71.1,90.7,  76.6,92.4,  92.7,94.2,  85.0,108.9,  100);
  chassis.follow(path3, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 12, 3);
  chassis.turnToHeading(-15, false, 12, akp, aki, akd, 1);
  delay(45000);
  stopAuto();
  delay(100000);
  return;
}

void awp()
{
  startAuto(131, 54, 0);

  Path path1(131, 59, 133.3, 104.5, 131.3, 115.2, 121.2, 123.3, 100);
  chassis.setOffset(0, 0);
  chassis.follow(path1, false, 12, 12, lkp * .6, lki, lkd, akp, aki, akd, 12, 12, 20);
  right_wing.set(true);
  Pose fake(path1.x3, path1.y3, 0);
  do
  {
    wait(10, vex::msec);
  } while (fabs(chassis.getPose().distance(fake)) > 10);
  intake.spinVolts(-12);
  delay(500);
  chassis.turnToHeading(-95, false, 12, akp, aki, akd, 5);
  chassis.turnToHeading(-45, false, 12, akp, aki, akd, 5);
  chassis.pid(-5, -45, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 2);
  right_wing.set(false);
  chassis.turnToHeadingUnbounded(110, false, 12, akp, aki, akd, 6);
  chassis.setOffset(0, 0);
  chassis.moveTo(115, 128, true, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 10);
  chassis.moveTo(97, 125, true, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 10);
  chassis.pid(10, 90, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 3);
  delay(500);
  chassis.pid(-50, 90, false, 4, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
  delay(500);


  //contest ball in neutral
  chassis.setOffset(0, 8);
  intake.spinVolts(12);
  Path path2(104.5, 126.1, 125.6, 126.4, 129.0, 102.5, 71.4, 95.6, 100);
  chassis.follow(path2, false, 12, 12, lkp * .6, lki, lkd, akp, aki, akd, 12, 18, 5);
  left_wing.set(true);
  chassis.turnToHeading(0, false, 12, akp, aki, akd, 5);
  right_wing.set(true);
  intake.spinVolts(-12);
  delay(400);
  chassis.pid(24, 0, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 8);
  delay(400);
  right_wing.set(false);
  left_wing.set(false);
  chassis.moveTo(85, 95.6, true, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 5);
  chassis.turnToHeading(180, false, 12, akp, aki, akd, 5);
  intake.spinVolts(2);
  chassis.moveTo(83, 74, false, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 5); // grab ball 1
  chassis.pid(-15, 180, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 2);
  chassis.moveTo(80.6, 112, false, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 5); // go into goal
  intake.spinVolts(-12);
  chassis.pid(-10, 0, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 2);
  intake.spinVolts(2);
  chassis.moveTo(93, 76, false, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 5); // grab ball 2
  chassis.pid(-5, 180, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 2);
  chassis.moveTo(80.6, 112, false, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 5);
  intake.spinVolts(-12);
  delay(500);
  stopAuto();
  delay(100000);
  return;
}