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
  startAuto(131, 54, 0);
  Path path1(131,59,  129.6,123.6,  132.8,136.4,  98.2,136.5, 100);
  chassis.setOffset(0, 8);
  chassis.follow(path1, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 12, 10);
  intake.spinVolts(-12);
  delay(500);
  chassis.setOffset(0, 0);
  chassis.pid(-14, -90, false, 6, 12, lkp, lki, lkd, akp, aki, akd, .4, 2);
  chassis.turnToHeading(98, false, 12, akp, aki, akd, 1);
  chassis.pid(-50, 98, false, 3, 12, lkp, lki, lkd, akp, aki, akd, 12, 0); 
  delay(500);
  Path path2(98.2,131.9,  142.8,116.6,  72.3,69.7,  59.0,104, 100);
  chassis.follow(path2, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 12, 3);
  delay(500);
  chassis.turnToHeading(15, false, 12, akp, aki, akd, 1);
  chassis.pid(4.5, 15, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 3); 
  delay(500);
  chassis.turnToHeading(15, false, 12, akp, aki, akd, 1);
  printTime();
  return;
}