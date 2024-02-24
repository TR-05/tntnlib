#include "autos.h"
#include "vex.h"
using namespace tntnlib;

void delay(float ms)
{
  vex::wait(ms, vex::msec);
}

void baseMatchAuto();

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
  chassis.tank(0, 0, 1, 0, 100, 0);
  intake.stop(vex::brakeType::coast);
  printTime();
}

void touchAWP()
{
  while (getTime() < 40.25)
  {
    wait(10, vex::msec);
  }
  chassis.moveTo(110.5, 98, true, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 5);
  chassis.turnToHeading(165, false, 12, akp, aki, akd, 5);
  intake.spinVolts(4);
  chassis.pid(50, 165, false, 6, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
  delay(750);
  chassis.pid(-6, 190, false, 6, 12, lkp, lki, lkd, akp, aki, akd, 12, 3);
  chassis.turnToHeading(-8, false, 12, akp, aki, akd, 5);
  chassis.pid(-50, -8, false, 6, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
  intake.spinVolts(-8);
  delay(800);
  chassis.turnToHeading(27.5, false, 12, akp * 1.5, aki, akd, 0);
  delay(500);
  stopAuto();
  delay(100000);

}

void programming_skills2()
{
   startAuto(131, 54, 0);
  chassis.breakOutTime = 3.5;
  Path path1(131,59,  133.3,104.5,  129.6,112.0,  102,126,  100);
  chassis.setOffset(0, 0);
  chassis.follow(path1, false, 12, 12, lkp * .9, lki, lkd, akp, aki, akd, 12, 16, 0);
  delay(500);
  right_wing.set(true);
  left_wing.set(true);
  delay(500);
  intake.spinVolts(-12);
  chassis.LineWait(path1.x3, path1.y3, 5, 3000);
  Path path2(98.2, 131.9, 142.8, 116.6, 72.3, 69.7, 55.0, 104, 100);
  chassis.follow(path2, true, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 12, 3);

}



void programming_skills()
{
  startAuto(131.75, 54 + 0, 0);
  Path path1(131, 59, 129.6, 123.6, 132.8, 136.4, 98.2, 134.5, 100);
  chassis.setOffset(0, 8);
  chassis.follow(path1, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 12, 10);
  intake.spinVolts(-12);
  delay(300);
  chassis.setOffset(0, 0);
  /*
  chassis.pid(-7, -90, false, 6, 12, lkp, lki, lkd, akp, aki, akd, .4, 2);
  chassis.turnToHeading(98, false, 12, akp, aki, akd, 1);
  chassis.pid(-50, 98, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
  delay(500);
  */
  Path path2(98.2, 131.9, 142.8, 116.6, 72.3, 69.7, 55.0, 104, 100);
  chassis.follow(path2, true, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 12, 3);
  left_wing.set(1);
  right_wing.set(1);

  delay(500);
  left_wing.set(0);
  right_wing.set(0);
  chassis.turnToHeading(15, false, 12, akp, aki, akd, 3);
  chassis.pid(7, 15, false, 2, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
  right_wing.set(1);
  while (getTime() < 29.5)
  {
    delay(10);
  }
    printTime();
  chassis.moveTo(63, 91.6, true, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 5);
  chassis.turnToPose(65, 107, false, 12, akp, aki, akd, 7);
  left_wing.set(1);
  right_wing.set(1);
  chassis.moveTo(65, 110, false, 12, 12, lkp*1.3, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 8);
  delay(300);

  chassis.moveTo(70.9, 91, true, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 2);
  left_wing.set(0);
  right_wing.set(0);
  Path path3(71.1, 90.7, 76.6, 92.4, 92.7, 94.2, 81, 106, 100);
  chassis.follow(path3, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 12, 3);
  intake.spinVolts(-12);
  chassis.turnToHeading(-15, false, 12, akp, aki, akd, 3);
  chassis.pid(7, -15, false, 2, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
  left_wing.set(1);
  right_wing.set(1);
  printTime();
  while (getTime() < 57.5)
  {
    delay(10);
  }
  left_wing.set(1);
  right_wing.set(1);
  chassis.pid(-4, 0, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 2);
  chassis.pid(9, 0, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
  delay(500);
  chassis.pid(-7, 0, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 3);
  delay(2000);
  stopAuto();
  delay(100000);
  return;
}

void ElimAwp()
{
  baseMatchAuto();
  while (getTime() < 45.5)
  {
    wait(10, vex::msec);
  }
  stopAuto();
  delay(100000);
  return;
}


void awp()
{
  baseMatchAuto();
  touchAWP();
  return;
}

void baseMatchAuto()
{
   startAuto(131, 54, 0);
  chassis.breakOutTime = 3.5;
  Path path1(131,59,  133.3,104.5,  129.6,112.0,  122,118.3,  100);
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
  intake.spinVolts(-12);
  chassis.pid(5, -38, false, 5, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
  delay(700);
  chassis.turnToHeading(-95, false, 12, akp, aki, akd, 5);
  right_wing.set(false);
  left_wing.set(false);
  chassis.turnToHeading(-45, false, 12, akp, aki, akd, 5);
  chassis.pid(-5, -45, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 2);

  chassis.turnToHeadingUnbounded(110, false, 12, akp, aki, akd, 6);
  chassis.setOffset(0, 0);
  chassis.moveTo(115, 128, true, 10, 12, lkp, lki, lkd, akp, aki, akd * 1.25, .4, 12);
  chassis.moveTo(90, 126, true, 12, 12, lkp * 1.4, lki, lkd, akp, aki, akd * 1.25, 12, 20); // shove 1
  delay(400);
  chassis.pid(6, 90, false, 12, 12, lkp, lki, lkd, akp, aki, akd, .2, 0);
  delay(500);
  chassis.pid(-50, 90, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
  delay(900);



  // contest ball in neutral and shove it
  chassis.setOffset(0, 8);
  intake.spinVolts(12);
  chassis.breakOutTime = 10;
  Path path2(104.5, 126.1, 143.7, 134.2, 132.2, 105.7, 71.7, 94.8, 100);
  chassis.follow(path2, false, 12, 12, lkp * .6, lki, lkd, akp, aki, akd * 1.25, .4, 22, 6);
  chassis.breakOutTime = 5;
  chassis.setOffset(0, 8);
  left_wing.set(true);
  chassis.turnToHeading(0, false, 12, akp, aki, akd, 6);
  right_wing.set(true);
  intake.spinVolts(-12);
  chassis.pid(23, 0, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
  delay(900);
  right_wing.set(false);
  left_wing.set(false);

  // grab extra 2 balls from 24
  chassis.moveTo(86, 95.6, true, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 5); // back out of goal
  chassis.turnToHeading(180, false, 12, akp, aki, akd * 1.6, 10);
  intake.spinVolts(4);
  delay(1000);
  chassis.moveTo(85, 77, false, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 5); // grab ball 1
  chassis.pid(-15, 180, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 5);
  chassis.moveTo(80.6, 116, false, 12, 12, lkp, lki, lkd, akp * 1.0, aki, akd * 1.8, 12, 10); // go into goal
  intake.spinVolts(-12);


  chassis.turnToHeading(-20, false, 12, akp, aki, akd, 5);
  chassis.pid(6, -20, false, 3, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);

  // stopAuto();
  //  get awp pole touch
  printTime();
}



void safeBaseMatchAuto()
{
  
   startAuto(131, 54, 0);
  chassis.breakOutTime = 3.5;
  Path path1(131,59,  133.3,104.5,  129.6,112.0,  122,118.3,  100);
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
  intake.spinVolts(-12);
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
  intake.spinVolts(12);
  chassis.breakOutTime = 10;

  Path path2(104.5,126.1,  154.9,128.4,  108.0,99.9,  84.4,106.0,  100);
  chassis.follow(path2, false, 12, 12, lkp * .6, lki, lkd, akp, aki, akd * 1.25, .4, 22, 6);
  chassis.turnToPose(80.6, 116, false, 12, akp, aki, akd, 5);
  chassis.moveTo(80.6, 116, false, 12, 12, lkp, lki, lkd, akp * 1.0, aki, akd * 1.8, 12, 10); // go into goal
  intake.spinVolts(-12);


  chassis.turnToHeading(-20, false, 12, akp, aki, akd, 5);
  chassis.pid(7, -20, false, 3, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);

  // stopAuto();
  //  get awp pole touch
  printTime();
}

void safeAWP()
{
  safeBaseMatchAuto();
  touchAWP();
}

void safeElim()
{
  safeBaseMatchAuto();
  while (getTime() < 45.5)
  {
    wait(10, vex::msec);
  }
  stopAuto();
  delay(100000);
  return;
}