#include "autos.h"

#include "vex.h"
//#include "vision.h"
using namespace tntnlib;

void delay(float ms) { vex::wait(ms, vex::msec); }

float startTime{0}, endTime{0}, totalTime{0};
void baseMatchAuto();

/* Example auto move functions
  startAuto(0,0,0);
  chassis.tuneOffsets(3600, akp, aki, akd, 6, 2); //Tunes odom + imu constants:
  DISABLE TERMINAL PRINT then jig bot to a tile, run auto, once bot stops moving
  tap and hold brain screen return;

  chassis.pid(24, 60, 60, false, 12, 12, lkp, lki, lkd, akp, aki, akd, .4, 2);
  //Moves forward 24 inches looking at xy point (60,60) chassis.pid(-24, 0,
  true, 12, 12, lkp, lki, lkd, akp, aki, akd, .4, 2); //Moves backward 24 inches
  holding heading 0,0 chassis.SwingOnLeftToHeading(-45, 0, 12, akp*1.6, aki,
  akd, 1); chassis.moveTo(0, 20, false, 12, 12, lkp, lki, lkd, akp*1.3, aki, akd
  * 1.25, 12, 2); chassis.boomerangTo(0, 20, -180, false, 12, 12, lkp, lki, lkd,
  akp, aki, akd, .2, .5, 12, 2); chassis.turnToHeading(0, false, 12, akp, aki,
  akd, 1);
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
float shotCount = 0;
void stopAuto()
{
    printTime();
    chassis.stateMachineOff();
    delay(20);
    chassis.tank(0, 0, 1, 0, 100, 0);
    flywheel.stop(vex::brakeType::coast);
    intakeVolts = 0;
    FWrpm = 0;
    delay(100000);
}
void programming_skills()
{
    chassis.breakOutTime = 10;
    startAuto(34.6, 13.2, -90);
    FWrpm = 0;
    intakeVolts = 0;
    // Score Preload
    chassis.pid(-50, -90, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
    delay(400);

    // Go to matchload
    chassis.moveTo(24.5, 26, false, 8, 10, lkp * 1.3, lki, lkd, akp * 1.5, aki, akd * 1.25, 12, 3);
    aligner.set(0);
    chassis.turnToHeading(-135, false, 12, akp, aki, akd, 5);
    spaceMaker.set(1);
    intakeVolts = (7);
    left_intake_piston.set(1);
    right_intake_piston.set(1);
    chassis.autoTankPct(100, 100);
    delay(1500);

    // Aim for matchload
    FWrpm = 3800;
    chassis.autoTankVolts(-6, 6);
    delay(100);
    intakeVolts = (0);
    delay(900);
    //offset = 20;
    // visionControl = true;
    chassis.turnToHeading(-153.5, false, 12, akp, aki, akd, 0);
    // matchload 23 balls
    delay(900);
    intakeVolts = (12);
    loadMacro(26, 600, 300);
    delay(500);
    FWrpm = 0;
    intakeVolts = -6;
    delay(10);
    chassis.stateMachineOn();

    // exit matchload and raise spaceMaker
    spaceMaker.set(0);
    chassis.turnToHeading(-135, false, 12, akp, aki, akd, 0);
    intakeVolts = (0);
    delay(600);
    chassis.autoTankPct(-100, -100);
    delay(800);

    // Switch Sides
    Path path1(14.1, 15.6, 29.4, 47.2, 54.1, 41.2, 61.1, 40.1, 100);
    chassis.follow(path1, true, 12, 12, lkp * 1.1, lki, lkd, akp, aki, akd, 12, 15, 5);
    chassis.turnToHeading(90, false, 12, akp, aki, akd, 12);
    intakeVolts = (7);
    aligner.set(1);
    Path path3(61.4, 40.1, 84.7, 37.7, 92.2, 38.0, 109, 18, 100);
    chassis.follow(path3, false, 7.5, 12, lkp * .8, lki, lkd, akp * 1.3, aki, akd, .4, 13, 5);
    delay(300);

    // Drive into Matchloader
    chassis.turnToHeading(135, false, 12, akp, aki, akd, 5);
    aligner.set(1);
    spaceMaker.set(1);
    chassis.autoTankPct(100, 100);
    delay(1700);

    // Aim for matchload
    FWrpm = 3800;
    chassis.autoTankVolts(6, -6);
    delay(1000);
    //offset = -7.5;
    chassis.turnToHeading(155, false, 12, akp, aki, akd, 0);
    intakeVolts = (12);

    printTime();
    while (getTime() < 61)
    {
        loadMacro(1, 600, 300);
        shotCount += 1;
    }
    printTime();
    return;
}

void elimAwp()
{
    baseMatchAuto();
    while (getTime() < 50)
    {
        loadMacro(1, 1100, 300);
        shotCount += 1;
    }
    stopAuto();
    return;
}

void awp()
{
    baseMatchAuto();
    while (getTime() < 39)
    {
        loadMacro(1, 1100, 300);
        shotCount += 1;
    }
    delay(500);
    FWrpm = 0;
    intakeVolts = -6;
    delay(10);
    chassis.stateMachineOn();

    // exit matchload and raise spaceMaker
    spaceMaker.set(0);
    chassis.turnToHeading(135, false, 12, akp, aki, akd, 0);
    intakeVolts = (0);
    delay(600);
    chassis.autoTankPct(-100, -100);
    delay(800);
    chassis.autoTankPct(0, 0);
    chassis.moveTo(105, 48, true, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 5);
    chassis.turnToHeading(226, false, 12, akp, aki, akd, 6);
    chassis.moveTo(114, 58.6, true, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 5);
    chassis.turnToHeading(-116, false, 12, akp, aki, akd, 6);
    chassis.pid(-5, -116, false, 12, 12, lkp, lki, lkd, akp * 1.4, aki, akd, 12, 0);
    spaceMaker.set(1);
    delay(800);
    stopAuto();
    return;
}

void baseMatchAuto()
{
    chassis.breakOutTime = 10;
    startAuto(129.9, 38.0, -90);
    FWrpm = 0;
    intakeVolts = 0;
    // Grab first triball and pass it
    Path path1(129.9, 38.0, 114.3, 38.3, 104.3, 41.8, 89, 47, 100);
    left_intake_piston.set(true);
    right_intake_piston.set(true);
    intakeVolts = (5);
    chassis.follow(path1, false, 12, 12, lkp, lki, lkd, akp * 0.7, aki, akd, .4, 25, 3);
    FWrpm = 2050;
    left_intake_piston.set(false);
    right_intake_piston.set(false);
    intakeVolts = (3);
    spaceMaker.set(true);
    chassis.pid(-5, -90, false, 12, 12, lkp * 1.3, lki, lkd, akp, aki, akd, 12, 3);
    chassis.turnToHeading(-190, false, 12, akp, aki, akd, 5);
    intakeVolts = (12);
    delay(800);

    // grab second ball and pass it
    FWrpm = 2050;
    chassis.turnToHeading(-90, false, 12, akp, aki, akd, 10);
    left_intake_piston.set(true);
    right_intake_piston.set(true);
    intakeVolts = (4);
    chassis.moveTo(80, 48.3, false, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 4);
    left_intake_piston.set(false);
    right_intake_piston.set(false);
    delay(300);
    chassis.pid(-16, -85, false, 12, 12, lkp * 1.3, lki, lkd, akp, aki, akd, 12, 4);
    intakeVolts = (0);
    chassis.turnToHeading(-190, false, 12, akp, aki, akd, 5);
    intakeVolts = (12);
    delay(600);
    spaceMaker.set(false);
    chassis.turnToHeading(90, false, 12, akp, aki, akd, 5);

    // grab third and forth balls and push
    Path path2(94.4, 44.1, 89.0, 44.4, 78.3, 43.5, 79.8, 69, 100);
    chassis.follow(path2, true, 7, 12, lkp, lki, lkd, akp, aki, akd, 12, 9, 7);
    chassis.SwingOnLeftToHeading(-226.7, false, 12, akp * 1.6, aki, akd, 5);
    chassis.pid(-5, -226.7, false, 12, 12, lkp * 1.3, lki, lkd, akp, aki, akd, 12, 3);
    spaceMaker.set(true);
    delay(400);
    chassis.pid(6, -226.7, false, 12, 12, lkp * 1.3, lki, lkd, akp, aki, akd, 12, 3);
    chassis.turnToHeading(-180, false, 12, akp, aki, akd, 6);
    spaceMaker.set(false);
    chassis.pid(-8, -180, false, 12, 12, lkp * 1.3, lki, lkd, akp, aki, akd, 1, 0);
    delay(700);

    // complete awp triball
    Path path3(84.1, 64.5, 90.7, 40.0, 103.7, 41, 118, 29, 100);
    chassis.follow(path3, false, 8, 12, lkp * 1.0, lki, lkd, akp, aki, akd, 12, 18, 5);
    delay(300);
    chassis.turnToHeading(-52, false, 12, akp, aki, akd, 3);
    spaceMaker.set(true);
    delay(450);
    chassis.pid(-4, -52, false, 12, 12, lkp, lki, lkd, akp, aki, akd, .5, 0);
    delay(500);
    chassis.turnToHeading(65, false, 12, akp, aki, akd, 5);  // flick the ball out
    spaceMaker.set(0);
    delay(300);
    chassis.moveTo(108, 12, true, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 10);
    chassis.moveTo(94, 8, true, 12, 12, lkp * 1.5, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 10);  // first shove

    chassis.pid(4, 95, false, 12, 12, lkp, lki, lkd, akp, aki, akd, .5, 0);
    delay(500);
    chassis.pid(-50, 90, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);  // second shove
    delay(500);

    // Go to matchload
    chassis.moveTo(114, 26.3, false, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 4);
    chassis.turnToHeading(135, false, 12, akp, aki, akd, 5);
    aligner.set(1);
    spaceMaker.set(1);
    intakeVolts = (3.5);
    chassis.autoTankPct(100, 100);
    left_intake_piston.set(true);
    right_intake_piston.set(true);
    delay(1600);

    // Aim for matchload
    FWrpm = 3800;
    chassis.autoTankVolts(6, -6);
    delay(1000);
    chassis.turnToHeading(154, false, 12, akp, aki, akd, 0);

    // prep match loading
    printTime();
    delay(400);
    intakeVolts = 12;
    left_intake_piston.set(false);
    right_intake_piston.set(false);

    // continue in AWP or ELIM
}
