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
  chassis.moveTo(0, 20, false, 12, 12, lkp, lki, lkd, akp*1.3, aki, akd * 1.25, .3, 12, 2);
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
  totalTime = endTime - startTime;
  return totalTime / 1000.0;
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

void programming_skills()
{
  startAuto(8, 8, 0);

Path path(11.3,13.7,  46.1,59.8,  95.0,52.6,  130.7,13.7,  100);
chassis.follow(path, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 0, 12, 12, 8);

chassis.setOffset(0,-6);
Path path2(130.9,14.1,  141.3,144.0,  124.3,141.5,  11.1,14.1,  100);
chassis.follow(path2, true, 12, 12, lkp, lki, lkd, akp, aki, akd, 0, 12, 12, 3);
  //chassis.setOffset(6, 6);
  //chassis.moveTo(0, 24, false, 12, 12, lkp, lki, lkd, akp*1.3, aki, akd * 1.25, .3, 12, 2);
  delay(1000);

  // chassis.follow(path4, false, 9, 12, lkp, lki, lkd, akp, aki, akd, 0, 12, 12, 14);
  // chassis.follow(path3, false, 9, 12, lkp, lki, lkd, akp, aki, akd, 0, 12, 12, 3);
  chassis.turnToHeading(0, false, 12, akp, aki, akd, 1);

  printTime();
  return;
}