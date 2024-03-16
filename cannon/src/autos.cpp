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

void programming_skills()
{
    chassis.breakOutTime = 10;
    startAuto(34.6, 13.2, -90);
    FWrpm = matchloadRPM;
    intakeVolts = 0;
    // Score Preload
    chassis.pid(-50, -90, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
    delay(300);

    // Go to matchload
    chassis.moveTo(22.5, 21.5, false, 12, 12, lkp * 1.3, lki, lkd, akp * 1.5, aki, akd * 1.25, 12, 5);
    alignerL.set(1);
    alignerR.set(0);
    chassis.turnToHeading(-135, false, 12, akp, aki, akd, 12);
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

    while (getRunTime() < 4) // temp values tbd if necessary
    {
        delay(10);
    }
    intakeVolts = 12;
    pneumIntake(0);
    delay(200);

    while (getRunTime() < 23)
    {
        // loadMacro(1, 440, 270);
        loadMacro(1, 500, 270);
        shotCount += 1;
    }

    // escape sequence
    FWrpm = 0;
    intakeVolts = 0;
    alignerL.set(0);
    alignerR.set(0);
    Pose saved(9.7, 17.1, -147.15);
    chassis.setPose(saved);
    chassis.pid(6, -135, false, 12, 12, lkp * 3, lki, lkd, akp * 3, aki, akd, 12, 0);
    spaceMaker(0);
    delay(450);
    chassis.moveTo(53.3, 44, true, 12, 12, lkp * 1.3, lki, lkd, akp * 1.5, aki, akd * 1.25, 12, 7);

    // grab that one ball and shoot it hopefully over
    intakeVolts = 4;
    pneumIntake(1);
    chassis.turnToPose(64, 41, false, 12, akp, aki, akd, 12);
    chassis.moveTo(65, 41, false, 12, 12, lkp * 1.7, lki, lkd, akp * 1.5, aki, akd * 1.25, 12, 5);

    // bezier over to the matchloader
    Path path1(64, 41, 97.3, 40.8, 103, 35.0, 121, 8.5, 100);
    FWrpm = 3800;
    chassis.follow(path1, false, 11, 12, lkp * 1.2, lki, lkd * .7, akp * 1.3, aki, akd * 1.4, 12, 25, 0);
    pneumIntake(0);
    alignerL.set(0);
    alignerR.set(1);
    chassis.LineWait(path1.x3, path1.y3, 30, 5000);
    intakeVolts = 12;
    chassis.LineWait(path1.x3, path1.y3, 8, 5000);
    FWrpm = matchloadRPM;

    intakeVolts = (7);
    left_intake_piston.set(1);
    right_intake_piston.set(1);
    chassis.pid(200, 135, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
    delay(1600);
    intakeVolts = (2);
    chassis.turnToHeading(160, false, 12, akp * 1.5, aki, akd, 0);
    spaceMaker(1);
    // matchload 23 balls
    delay(900);
    antijam = true;
    intakeVolts = 12;
    pneumIntake(0);
    delay(200);
    while (getRunTime() < 54)
    {
        loadMacro(1, 500, 270);
        // loadMacro(1, 440, 270);
        shotCount += 1;
    }

    // prep zone leave
    alignerL.set(0);
    alignerR.set(0);
    chassis.setPose(Pose(131.6, 14.7, chassis.getPose().theta)); // reset to absolute coords
    chassis.pid(6, 135, false, 12, 12, lkp * 3, lki, lkd, akp * 3, aki, akd, 12, 0);
    spaceMaker(0);
    delay(800);

    // escape zone
    chassis.pid(-30, 135, false, 12, 12, lkp * 1.5, lki, lkd, akp, aki, akd, 12, 6);
    Path hangPath(129.0, 78.0, 137.4, 31.1, 118.7, 28.5, 114.6, 27.1, 100);
    chassis.follow(hangPath, true, 12, 12, lkp * 4, lki, lkd, akp * 2, aki, akd, 12, 15, 0);
    chassis.LineWait(hangPath.x3, hangPath.y3, 40, 5000);
    delay(5000);

    stopAuto();
    delay(100000);
}

void elimsAuto()
{
    baseMatchAuto();
    while (getRunTime() < 44.9)
    {
        loadMacro(1, 1100, 400);
        shotCount += 1;
    }
    pneumIntake(1);
    stopAuto();
    return;
}

void awp()
{
    baseMatchAuto();
    // matchload around 15 balls and exit with time to spare
    while (getRunTime() < 39.5)
    {
        loadMacro(1, 900, 350);
        shotCount += 1;
    }

    // prep zone leave
    alignerL.set(0);
    alignerR.set(0);
    chassis.setPose(Pose(131.6, 14.7, chassis.getPose().theta)); // reset to absolute coords
    chassis.pid(6, 135, false, 12, 12, lkp * 3, lki, lkd, akp * 3, aki, akd, 12, 0);
    spaceMaker(0);
    delay(800);

    // escape zone
    chassis.pid(-30, 135, false, 12, 12, lkp * 1.5, lki, lkd, akp, aki, akd, 12, 9);

    // lineup for AWP touch
    intakeVolts = 0;
    chassis.moveTo(107, 57, true, 12, 12, lkp * 1.3, lki, lkd, akp * 1.5, aki, akd * 1.25, 12, 5);
    chassis.turnToHeading(-85, false, 12, akp, aki, akd, 10);
    chassis.moveTo(112, 59, true, 12, 12, lkp * 1.3, lki, lkd, akp * 1.5, aki, akd * 1.25, 12, 6);
    spaceMaker(1);
    delay(300);
    chassis.turnToHeading(-120, false, 8, akp, aki, akd, 0);
    delay(1000);
    stopAuto();


    // touch bar
    spaceMaker(1);
    delay(10);
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
    Path path1(129.9, 38.0, 114.3, 38.3, 104.3, 42.5, 94, 46, 100);
    pneumIntake(1);
    intakeVolts = (5);
    chassis.follow(path1, false, 12, 12, lkp * 1.5, lki, lkd * 1.4, akp * 1.0, aki, akd, .4, 25, 3);
    FWrpm = 3500;
    pneumIntake(0);
    intakeVolts = (3);
    chassis.pid(-2, -90, false, 12, 12, lkp * 1.5, lki, lkd, akp, aki, akd, 12, 1);
    chassis.turnToHeading(-195, false, 12, akp, aki, akd, 5);
    intakeVolts = (12);
    delay(800);

    // grab second ball and pass it
    FWrpm = 3500;
    pneumIntake(1);
    chassis.turnToHeading(-98, false, 12, akp, aki, akd, 10);
    intakeVolts = (4);
    chassis.moveTo(81.5, 48, false, 12, 12, lkp * 1.5, lki, lkd * 1.5, akp * 1.3, aki, akd * 1.25, 12, 4);
    pneumIntake(0);
    delay(300);
    chassis.pid(-16, -85, false, 12, 12, lkp * 1.3, lki, lkd, akp, aki, akd, 12, 4);
    intakeVolts = (0);
    chassis.turnToHeading(-195, false, 12, akp, aki, akd, 5);
    intakeVolts = (12);
    delay(600);
    FWrpm = 0;
    chassis.turnToHeading(90, false, 12, akp, aki, akd, 5);

    // grab third and forth balls and push
    Path path2(94.4, 44.1, 89.0, 44.4, 81.3, 43.5, 83, 72, 100);
    chassis.follow(path2, true, 7, 12, lkp, lki, lkd, akp, aki, akd, 12, 9, 9);
    delay(400);
    chassis.SwingOnLeftToHeading(-226.7, false, 12, akp * 1.6, aki, akd, 5);
    chassis.pid(-5, -226.7, false, 12, 12, lkp * 1.3, lki, lkd, akp, aki, akd, 12, 3);
    spaceMaker(1);
    delay(400);
    chassis.pid(6, -226.7, false, 12, 12, lkp * 1.3, lki, lkd, akp, aki, akd, 12, 3);
    chassis.turnToHeading(-180, false, 12, akp, aki, akd, 6);
    spaceMaker(0);
    chassis.pid(-12, -180, false, 12, 12, lkp * 1.3, lki, lkd, akp, aki, akd, 1, 0);
    delay(900);

    // complete awp triball
    Path path3(84.1, 64.5, 90.7, 40.0, 101.7, 50.1, 114.4, 30, 100);
    FWrpm = matchloadRPM;
    pneumIntake(1);
    intakeVolts = 3;
    chassis.follow(path3, false, 12, 12, lkp * 0.8, lki, lkd*1.2, akp, aki, akd, 12, 18, 5);
    pneumIntake(0);
    spaceMaker(1);
    delay(400);

    // shoot passing play
    chassis.turnToHeading(158, false, 12, akp, aki, akd, 3);
    delay(200);
    intakeVolts = 12;
    delay(800);
    spaceMaker(0);
    FWrpm = 0;
    delay(800);

    chassis.turnToHeading(-45, false, 12, akp, aki, akd, 5);
    delay(400);
    chassis.moveTo(124, 22.25, true, 12, 12, lkp * 1.4, lki, lkd * 1.5, akp * 1.3, aki, akd * 1.25, 12, 3);
    spaceMaker(1);
    delay(800);

    // score the red triball
    chassis.pid(18.5, -45, false, 12, 12, lkp, lki, lkd, akp, aki, akd, .5, 3);
    chassis.SwingOnLeftToHeading(42, false, 12, akp * 1.4, aki, akd * 1.6, 5);
    chassis.SwingOnRightToHeading(63, false, 12, akp * 1.4, aki, akd * 1.6, 5);
    delay(100);
    spaceMaker(0);
    delay(350);
    Path awpBallPush(118.6, 23, 112.9, 10.9, 104.8, 12.4, 98, 11.5, 100);
    //chassis.follow(awpBallPush, true, 12, 12, lkp * 1.4, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 10, 10);
    chassis.moveTo(98, 12.5, true, 12, 12, lkp * 1.4, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 10);
    delay(750);

    // Go to matchload
    chassis.moveTo(117.5, 22, false, 12, 12, lkp*1.4, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 4);
    chassis.turnToHeading(135, false, 12, akp, aki, akd, 5);
    alignerL.set(0);
    alignerR.set(1);
    FWrpm = matchloadRPM;

    intakeVolts = (7);
    left_intake_piston.set(1);
    right_intake_piston.set(1);
    chassis.pid(200, 135, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
    delay(1400);
    intakeVolts = (2);
    chassis.turnToHeading(153, false, 12, akp * 1.5, aki, akd, 0);
    spaceMaker(1);
    // matchload 12 balls
    delay(900);
    antijam = true;
    intakeVolts = 12;
    pneumIntake(0);
    delay(200);
    while (getRunTime() < 20) // temp values tbd if necessary
    {
        delay(10);
    }
    intakeVolts = 12;
    pneumIntake(0);
    delay(800);

    // continue in AWP or ELIM
}
