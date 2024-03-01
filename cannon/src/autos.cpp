#include "autos.h"

#include "vex.h"
// #include "vision.h"
using namespace tntnlib;

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
    endTimer();
    printf("Time: %.2f\n", totalRunTime);
}

void startAuto(float x, float y, float theta)
{
    printf("Entered Auto\n");
    startTimer();
    chassis.initialize(false, x, y, theta);
    chassis.setOffset(0, 0);
    chassis.stateMachineOn();
    chassis.breakOutTime = 100000;
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
void spaceMaker(bool state)
{
    spaceMakerL.set(state);
    spaceMakerR.set(state);
}
void pneumIntake(bool state)
{
    left_intake_piston.set(state);
    right_intake_piston.set(state);
}

void programming_skills2()
{
    chassis.breakOutTime = 10;
    startAuto(34.6, 13.2, -90);
    FWrpm = 3600;
    intakeVolts = 0;
    // Score Preload
    chassis.pid(-50, -90, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
    delay(300);

    // Go to matchload
    chassis.moveTo(22.5, 21.5, false, 12, 12, lkp * 1.3, lki, lkd, akp * 1.5, aki, akd * 1.25, 12, 4);
    alignerL.set(1);
    alignerR.set(0);
    chassis.turnToHeading(-135, false, 12, akp, aki, akd, 8);
    spaceMaker(1);
    intakeVolts = (7);
    left_intake_piston.set(1);
    right_intake_piston.set(1);
    chassis.pid(100, -135, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
    delay(1100);
    intakeVolts = (0);
    chassis.turnToHeading(-153, false, 12, akp, aki, akd, 0);
    // matchload 23 balls
    delay(900);
    intakeVolts = (12);
    antijam = true;
    while (getRunTime() < 22)
    {
        loadMacro(1, 440, 270);
        shotCount += 1;
    }
    Pose saved(9.7, 17.1, -147.15);
    chassis.setPose(saved);
    delay(400);
    FWrpm = 0;
    intakeVolts = 0;
    chassis.pid(4, -135, false, 12, 12, lkp*1.5, lki, lkd, akp, aki, akd, 12, 0);
    delay(150);
    chassis.moveTo(49.3, 44, true, 12, 12, lkp * 1.3, lki, lkd, akp * 1.5, aki, akd * 1.25, 12, 3);
    spaceMaker(0);
    intakeVolts = 3;
    pneumIntake(1);
    chassis.turnToPose(64, 41, false, 12, akp, aki, akd, 8);
    chassis.moveTo(64, 41, false, 12, 12, lkp * 1.3, lki, lkd, akp * 1.5, aki, akd * 1.25, 12, 3);
    Path path1(64,41,  97.3,43.8,  107.7,36.0,  120.5,8,  100);
    FWrpm = 2600;
    chassis.follow(path1, false, 12, 12, lkp * 1.1, lki, lkd, akp, aki, akd, 12, 15, 0);
    pneumIntake(0);
    alignerL.set(0);
    alignerR.set(1);
    chassis.LineWait(path1.x3, path1.y3, 20, 5000);
    spaceMaker(1);
    intakeVolts = 12;
    chassis.LineWait(path1.x3, path1.y3, 5, 5000);
    FWrpm = 3600;

    intakeVolts = (7);
    left_intake_piston.set(1);
    right_intake_piston.set(1);
    chassis.pid(100, 135, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
    delay(1400);
    intakeVolts = (2);
    chassis.turnToHeading(160, false, 12, akp, aki, akd, 0);
    // matchload 23 balls
    delay(900);
    intakeVolts = (12);
    antijam = true;
    while (getRunTime() < 54)
    {
        loadMacro(1, 440, 270);
        shotCount += 1;
    }
    delay(400);
    FWrpm = 0;
    intakeVolts = 0;
    chassis.pid(3, 135, false, 12, 12, lkp*1.5, lki, lkd, akp, aki, akd, 12, 0);
    delay(150);
    chassis.pid(-27, 135, false, 12, 12, lkp*1.5, lki, lkd, akp, aki, akd, 12, 4);
    chassis.moveTo(119.4,7, false, 12, 12, lkp * 1.3, lki, lkd, akp * 1.5, aki, akd * 1.25, 12, 4);
    chassis.moveTo(131.4,29, true, 12, 12, lkp * 1.3, lki, lkd, akp * 1.5, aki, akd * 1.25, 12, 4);
    chassis.pid(-50, 180, false, 12, 12, lkp*1.5, lki, lkd, akp, aki, akd, 12, 0);
    delay(2000);

    //119.4 7
    stopAuto();
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
    alignerL.set(0);
    alignerR.set(0);
    chassis.turnToHeading(-135, false, 12, akp, aki, akd, 5);
    spaceMakerL.set(1);
    spaceMakerR.set(1);
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
    // offset = 20;
    //  visionControl = true;
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
    spaceMakerL.set(0);
    spaceMakerR.set(0);
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
    alignerL.set(1);
    alignerR.set(1);
    Path path3(61.4, 40.1, 84.7, 37.7, 92.2, 38.0, 109, 18, 100);
    chassis.follow(path3, false, 7.5, 12, lkp * .8, lki, lkd, akp * 1.3, aki, akd, .4, 13, 5);
    delay(300);

    // Drive into Matchloader
    chassis.turnToHeading(135, false, 12, akp, aki, akd, 5);
    alignerL.set(1);
    alignerR.set(1);
    spaceMakerL.set(1);
    spaceMakerR.set(1);
    chassis.autoTankPct(100, 100);
    delay(1700);

    // Aim for matchload
    FWrpm = 3800;
    chassis.autoTankVolts(6, -6);
    delay(1000);
    // offset = -7.5;
    chassis.turnToHeading(155, false, 12, akp, aki, akd, 0);
    intakeVolts = (12);

    printTime();
    while (getRunTime() < 61)
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
    while (getRunTime() < 50)
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
    while (getRunTime() < 39)
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
    spaceMakerL.set(0);
    spaceMakerR.set(0);
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
    spaceMakerL.set(1);
    spaceMakerR.set(1);
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
    spaceMakerL.set(true);
    spaceMakerR.set(true);
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
    spaceMakerL.set(false);
    spaceMakerR.set(false);
    chassis.turnToHeading(90, false, 12, akp, aki, akd, 5);

    // grab third and forth balls and push
    Path path2(94.4, 44.1, 89.0, 44.4, 78.3, 43.5, 79.8, 69, 100);
    chassis.follow(path2, true, 7, 12, lkp, lki, lkd, akp, aki, akd, 12, 9, 7);
    chassis.SwingOnLeftToHeading(-226.7, false, 12, akp * 1.6, aki, akd, 5);
    chassis.pid(-5, -226.7, false, 12, 12, lkp * 1.3, lki, lkd, akp, aki, akd, 12, 3);
    spaceMakerL.set(true);
    spaceMakerR.set(true);
    delay(400);
    chassis.pid(6, -226.7, false, 12, 12, lkp * 1.3, lki, lkd, akp, aki, akd, 12, 3);
    chassis.turnToHeading(-180, false, 12, akp, aki, akd, 6);
    spaceMakerL.set(false);
    spaceMakerR.set(false);
    chassis.pid(-8, -180, false, 12, 12, lkp * 1.3, lki, lkd, akp, aki, akd, 1, 0);
    delay(700);

    // complete awp triball
    Path path3(84.1, 64.5, 90.7, 40.0, 103.7, 41, 118, 29, 100);
    chassis.follow(path3, false, 8, 12, lkp * 1.0, lki, lkd, akp, aki, akd, 12, 18, 5);
    delay(300);
    chassis.turnToHeading(-52, false, 12, akp, aki, akd, 3);
    spaceMakerL.set(true);
    spaceMakerR.set(true);
    delay(450);
    chassis.pid(-4, -52, false, 12, 12, lkp, lki, lkd, akp, aki, akd, .5, 0);
    delay(500);
    chassis.turnToHeading(65, false, 12, akp, aki, akd, 5); // flick the ball out
    spaceMakerL.set(0);
    spaceMakerR.set(0);
    delay(300);
    chassis.moveTo(108, 12, true, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 10);
    chassis.moveTo(94, 8, true, 12, 12, lkp * 1.5, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 10); // first shove

    chassis.pid(4, 95, false, 12, 12, lkp, lki, lkd, akp, aki, akd, .5, 0);
    delay(500);
    chassis.pid(-50, 90, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 0); // second shove
    delay(500);

    // Go to matchload
    chassis.moveTo(114, 26.3, false, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 4);
    chassis.turnToHeading(135, false, 12, akp, aki, akd, 5);
    alignerL.set(1);
    alignerR.set(1);
    spaceMakerL.set(1);
    spaceMakerR.set(1);
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
