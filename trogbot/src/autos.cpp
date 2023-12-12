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
float akp = angularSettings.kP;
float aki = angularSettings.kI;
float akd = angularSettings.kD;
float lkp = linearSettings.kP;
float lki = linearSettings.kI;
float lkd = linearSettings.kD;

void printTime()
{
  endTime = Brain.timer(vex::msec);
  totalTime = endTime - startTime;
  printf("Time: %.2f\n", totalTime / 1000.0);
}
void startAuto(float x, float y, float theta)
{
  printf("Entered Auto\n");
  startTime = Brain.timer(vex::msec);
  chassis.initialize(false, x, y, theta);
  chassis.stateMachineOn();
  akp = angularSettings.kP;
  aki = angularSettings.kI;
  akd = angularSettings.kD;
  lkp = linearSettings.kP;
  lki = linearSettings.kI;
  lkd = linearSettings.kD;
}

void programming_skills()
{
  startAuto(12, 12, 45);

  Path path1(0, 0, 0, 24, 37, 37, 10, 48, 100);
  Path path2(10, 48, -23, 71, -72, 31, -42, 25, 100);
  Path path3(-42, 25, -4, 17, -70, 0, 0, 0, 100);
  Path path4 = path1 + path2;

  Path path(12.0, 12.0, 49.0, 65.0, 95.0, 65.0, 132.0, 12.0, 100);
  chassis.follow(path, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 0, 12, 12, 3);

  // chassis.follow(path4, false, 9, 12, lkp, lki, lkd, akp, aki, akd, 0, 12, 12, 14);
  // chassis.follow(path3, false, 9, 12, lkp, lki, lkd, akp, aki, akd, 0, 12, 12, 3);
  chassis.turnToHeading(0, false, 12, akp, aki, akd, 1);

  printTime();
  return;
}