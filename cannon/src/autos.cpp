#include "autos.h"
#include "vision.h"
#include "vex.h"
using namespace tntnlib;

void delay(float ms)
{
  vex::wait(ms, vex::msec);
}

float startTime{0}, endTime{0}, totalTime{0};

/* Example auto move functions
  startAuto(0,0,0);
  chassis.tuneOffsets(3600, akp, aki, akd, 6, 2); //Tunes odom + imu constants: DISABLE TERMINAL PRINT then jig bot to a tile, run auto, once bot stops moving tap and hold brain screen
  return;

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

float rpm = 0;
float visionPow = 0;
bool visionControl = false;
int updateFlywheel()
{
  while (true)
  {
    if (rpm != 0)
      flywheel.spinRPM(rpm);
    else
      flywheel.stop(vex::brakeType::coast);
    if (visionControl)
    {
      visionPow = visionPower();
      chassis.stateMachineOff();
      chassis.tank(visionPow, -visionPow);
    }
    vex::wait(10, vex::msec);
  }
  return 0;
}

void stopAuto()
{
  chassis.stateMachineOff();
  delay(20);
  chassis.tank(0, 0);
  flywheel.stop(vex::brakeType::coast);
  intake.spinVolts(0);
  visionControl = false;
  rpm = 0;
  printTime();
  delay(100000);
}
void programming_skills()
{

  startAuto(34.6, 13.2, -90);
  vex::task fly(updateFlywheel);
  rpm = 0;

  // Score Preload
  chassis.pid(-50, -90, false, 3, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
  delay(400);

  // Go to matchload
  chassis.moveTo(24.5, 26, false, 8, 8, lkp, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 2);
  aligner.set(0);
  chassis.turnToHeading(-135, false, 12, akp, aki, akd, 5);
  spaceMaker.set(1);
  intake.spinVolts(7);
  left_intake_piston.set(1);
  right_intake_piston.set(1);
  chassis.autoTankPct(100, 100);
  delay(1300);

  // Aim for matchload
  rpm = 3600;
  chassis.autoTankVolts(-6, 6);
  delay(100);
  intake.spinVolts(0);
  delay(900);
  offset = 20;
  visionControl = true;

  // matchload 23 balls
  loadMacro(10, 800, 300);
  delay(500);
  rpm = 0;
  intake.spinVolts(-6);
  visionControl = false;
  delay(10);
  chassis.stateMachineOn();

  // exit matchload and raise spaceMaker
  spaceMaker.set(0);
  chassis.turnToHeading(-135, false, 12, akp, aki, akd, 0);
  intake.spinVolts(0);
  delay(600);
  chassis.autoTankPct(-100, -100);
  delay(800);

  // grab ball with spacemaker
  Path path1(14.1, 15.6, 29.4, 47.2, 54.1, 41.2, 61.1, 46.1, 100);
  chassis.follow(path1, true, 12, 12, lkp * 1.1, lki, lkd, akp, aki, akd, 12, 15, 20);
  spaceMaker.set(1);
  chassis.moveTo(path1.x3, path1.y3, true, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 5);
  chassis.turnToHeading(180, false, 12, akp, aki, akd, 8);
  // push 2 balls over
  chassis.pid(-13, 180, false, 6, 12, lkp, lki, lkd, akp, aki, akd, 12, 6);
  delay(250);
  chassis.pid(7, 180, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 2);
  spaceMaker.set(0);
  delay(200);
  // second
  chassis.pid(-50, 180, false, 10, 12, lkp, lki, lkd, akp, aki, akd, .5, 0);
  delay(650);

  intake.spinVolts(7);
  aligner.set(1);
  Path path3(61.9, 50.7, 74.0, 29.4, 85.2, 60.1, 125.3, 6.0, 100);
  chassis.follow(path3, false, 12, 12, lkp, lki, lkd, akp, aki, akd, .4, 18, 20);
  chassis.autoTankPct(100, 100);
  delay(1000);
  chassis.turnToHeading(152, false, 12, akp, aki, akd, 0);
  delay(1000);

  printTime();
  return;
}

void awp()
{
  startAuto(129.9, 38.0, -90);
  vex::task fly(updateFlywheel);
  rpm = 0;
  Path path1(129.9, 38.0, 114.3, 38.3, 104.3, 41.8, 89.9, 47.4, 100);
  left_intake_piston.set(true);
  right_intake_piston.set(true);
  intake.spinVolts(4);
  chassis.follow(path1, false, 12, 12, lkp, lki, lkd, akp * 0.7, aki, akd, 12, 25, 5);
  rpm = 2100;
  left_intake_piston.set(false);
  right_intake_piston.set(false);
  spaceMaker.set(true);
  delay(400);
  chassis.pid(-5, -90, false, 12, 12, lkp, lki, lkd, akp, aki, akd, .5, 2);
  chassis.turnToHeading(-180, false, 12, akp, aki, akd, 5);
  intake.spinVolts(12);
  delay(800);

  // grab second ball
  rpm = 2000;
  chassis.turnToHeading(-90, false, 12, akp, aki, akd, 5);
  left_intake_piston.set(true);
  right_intake_piston.set(true);
  intake.spinVolts(4);
  chassis.moveTo(81.24, 48.5, false, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 2);
  delay(250);
  left_intake_piston.set(false);
  right_intake_piston.set(false);
  chassis.pid(-15, -90, false, 12, 12, lkp, lki, lkd, akp, aki, akd, .5, 2);
  intake.spinVolts(0);
  chassis.turnToHeading(-185, false, 12, akp, aki, akd, 5);
  intake.spinVolts(12);
  delay(600);
  spaceMaker.set(false);
  chassis.turnToHeading(90, false, 12, akp, aki, akd, 5);

  // grab third and forth balls and push
  Path path2(94.4, 44.1, 89.0, 44.4, 78.3, 42.6, 79.8, 67.4, 100);
  chassis.follow(path2, true, 7, 12, lkp, lki, lkd, akp, aki, akd, 12, 9, 5);
  chassis.SwingOnLeftToHeading(-226.7, false, 12, akp * 1.6, aki, akd, 3);
  chassis.pid(-5, -226.7, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 2);
  spaceMaker.set(true);
  delay(400);
  chassis.pid(4, -226.7, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 2);
  chassis.turnToHeading(-180, false, 12, akp, aki, akd, 5);
  spaceMaker.set(false);
  chassis.pid(-8, -180, false, 12, 12, lkp, lki, lkd, akp, aki, akd, .5, 0);
  delay(1000);

  // complete awp triball
  Path path3(84.1, 64.5, 90.7, 40.0, 106.0, 48.1, 118.5, 26, 100);
  chassis.follow(path3, false, 10, 12, lkp * .7, lki, lkd, akp, aki, akd, 12, 18, 5);
  delay(300);
  chassis.turnToHeading(-50, false, 12, akp, aki, akd, 5);
  spaceMaker.set(true);
  chassis.pid(-3, -50, false, 12, 12, lkp, lki, lkd, akp, aki, akd, .5, 1);
  delay(400);
  chassis.turnToHeading(65, false, 12, akp, aki, akd, 5);
  spaceMaker.set(0);
  delay(400);
  chassis.moveTo(98, 12, true, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 10);
  chassis.pid(3, 90, false, 12, 12, lkp, lki, lkd, akp, aki, akd, .5, 2);
  chassis.pid(-50, 90, false, 4, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);// INCREASE POWER WHEN DONE
  delay(500);

  stopAuto();

  return;
}