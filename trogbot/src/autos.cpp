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

void programming_skills()
{
  startAuto(23, 16, -90);
  chassis.SwingOnLeftToHeading(-135, 0, 12, akp * 1.6, aki, akd, 0);
  delay(500);
  chassis.autoTankVolts(2.5, 2.5);
  delay(250);
  chassis.pid(-3, -135, false, 12, 12, lkp, lki, lkd, akp, aki, akd, .4, 2);
  delay(100);
  chassis.autoTankPct(5, 5);
  delay(1000);
  chassis.autoTankVolts(-3.5, -3.5);
  delay(300);
  chassis.SwingOnLeftToHeading(-145, 0, 12, akp * 1.6, aki, akd, 0);
  delay(1000);

  /* matchload */
  delay(2000);

  // chassis.autoTankVolts(-3.5, -3.5); //switch to this once we have a matchload bar
  chassis.autoTankVolts(3.5, 3.5);
  delay(300);
  chassis.autoTankVolts(0, 0);
  delay(300);
  // chassis.setPose(18, 18, -135); //assumes the matchload bar acts as a reset
  Path path(14.3, 14.3, 34.8, 54.9, 62.1, 43.4, 103.4, 42.0, 100);
  chassis.follow(path, true, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 24, 4);

  chassis.turnToHeading(135, false, 12, akp, aki, akd, 10);
  // chassis.setOffset(0, 9);
  chassis.moveTo(128, 16, false, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 2);
  chassis.autoTankVolts(-3.5, -3.5);
  delay(500);
  chassis.SwingOnRightToHeading(145, 0, 12, akp * 1.6, aki, akd, 0);

  printTime();
  return;
}