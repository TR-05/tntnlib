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
        //loadMacro(1, 440, 270);
        loadMacro(1, 500, 270);
        shotCount += 1;
    }

    //escape sequence
    FWrpm = 0;
    intakeVolts = 0;
    alignerL.set(0);
    alignerR.set(0);
    Pose saved(9.7, 17.1, -147.15);
    chassis.setPose(saved);
    chassis.pid(6, -135, false, 12, 12, lkp*3, lki, lkd, akp*3, aki, akd, 12, 0);
    spaceMaker(0);
    delay(450);
    chassis.moveTo(53.3, 44, true, 12, 12, lkp * 1.3, lki, lkd, akp * 1.5, aki, akd * 1.25, 12, 7);

    //grab that one ball and shoot it hopefully over
    intakeVolts = 4;
    pneumIntake(1);
    chassis.turnToPose(64, 41, false, 12, akp, aki, akd, 12);
    chassis.moveTo(65, 41, false, 12, 12, lkp * 1.7, lki, lkd, akp * 1.5, aki, akd * 1.25, 12, 5);

    // bezier over to the matchloader
    Path path1(64,41,  97.3,40.8,  103,35.0,  121,8.5,  100);
    FWrpm = 3800;
    chassis.follow(path1, false, 11, 12, lkp * 1.2, lki, lkd*.7, akp*1.3, aki, akd*1.4, 12, 25, 0);
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
    chassis.turnToHeading(160, false, 12, akp*1.5, aki, akd, 0);
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
        //loadMacro(1, 440, 270);
        shotCount += 1;
    }

    //prep zone leave
    alignerL.set(0);
    alignerR.set(0);
    chassis.setPose(Pose(131.6, 14.7, chassis.getPose().theta)); // reset to absolute coords
    chassis.pid(6, 135, false, 12, 12, lkp*3, lki, lkd, akp*3, aki, akd, 12, 0);
    spaceMaker(0);
    delay(800);

    //escape zone
    chassis.pid(-30, 135, false, 12, 12, lkp * 1.5, lki, lkd, akp, aki, akd, 12, 6);
    Path hangPath(129.0,78.0,  137.4,31.1,  118.7,28.5,  114.6,27.1,  100);
    chassis.follow(hangPath, true, 12, 12, lkp*4, lki, lkd, akp*2, aki, akd, 12, 15, 0);
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
    //matchload around 15 balls and exit with time to spare
    while (getRunTime() < 39)
    {
        loadMacro(1, 1100, 400);
        shotCount += 1;
    }    

    //prep zone leave
    alignerL.set(0);
    alignerR.set(0);
    chassis.setPose(Pose(131.6, 14.7, chassis.getPose().theta)); // reset to absolute coords
    chassis.pid(6, 135, false, 12, 12, lkp*3, lki, lkd, akp*3, aki, akd, 12, 0);
    spaceMaker(0);
    delay(800);

    //escape zone
    chassis.pid(-30, 135, false, 12, 12, lkp * 1.5, lki, lkd, akp, aki, akd, 12, 6);
    
    //lineup for AWP touch
    intakeVolts = 0;
    chassis.moveTo(97, 54, true, 12, 12, lkp * 1.3, lki, lkd, akp * 1.5, aki, akd * 1.25, 12, 3);
    delay(300);
    chassis.turnToHeading(-125, false, 12, akp, aki, akd, 3);
    chassis.pid(-12, -125, false, 12, 12, lkp * 1.5, lki, lkd, akp, aki, akd, 12, 3);

    //touch bar
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
    Path path1(129.9, 38.0, 114.3, 38.3, 104.3, 41.8, 89, 47, 100);
    pneumIntake(1);
    intakeVolts = (5);
    chassis.follow(path1, false, 12, 12, lkp, lki, lkd, akp * 0.7, aki, akd, .4, 25, 3);
    FWrpm = 3800;
    pneumIntake(0);
    intakeVolts = (3);
    chassis.pid(-5, -90, false, 12, 12, lkp * 1.3, lki, lkd, akp, aki, akd, 12, 3);
    chassis.turnToHeading(-190, false, 12, akp, aki, akd, 5);
    intakeVolts = (12);
    delay(800);

    // grab second ball and pass it
    FWrpm = 3800;
    chassis.turnToHeading(-90, false, 12, akp, aki, akd, 10);
    pneumIntake(1);
    intakeVolts = (4);
    chassis.moveTo(80, 48.3, false, 12, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 4);
    pneumIntake(0);
    delay(300);
    chassis.pid(-16, -85, false, 12, 12, lkp * 1.3, lki, lkd, akp, aki, akd, 12, 4);
    intakeVolts = (0);
    chassis.turnToHeading(-190, false, 12, akp, aki, akd, 5);
    intakeVolts = (12);
    delay(600);
    FWrpm = 0;
    chassis.turnToHeading(90, false, 12, akp, aki, akd, 5);

    // grab third and forth balls and push
    Path path2(94.4, 44.1, 89.0, 44.4, 78.3, 43.5, 79.8, 69, 100);
    chassis.follow(path2, true, 7, 12, lkp, lki, lkd, akp, aki, akd, 12, 9, 7);
    chassis.SwingOnLeftToHeading(-226.7, false, 12, akp * 1.6, aki, akd, 5);
    chassis.pid(-5, -226.7, false, 12, 12, lkp * 1.3, lki, lkd, akp, aki, akd, 12, 3);
    spaceMaker(1);
    delay(400);
    chassis.pid(6, -226.7, false, 12, 12, lkp * 1.3, lki, lkd, akp, aki, akd, 12, 3);
    chassis.turnToHeading(-180, false, 12, akp, aki, akd, 6);
    spaceMaker(0);
    chassis.pid(-8, -180, false, 12, 12, lkp * 1.3, lki, lkd, akp, aki, akd, 1, 0);
    delay(700);

    // complete awp triball
    Path path3(84.1, 64.5, 90.7, 40.0, 103.7, 41, 118, 29, 100);
    chassis.follow(path3, false, 8, 12, lkp * 1.0, lki, lkd, akp, aki, akd, 12, 18, 5);
    delay(300);
    chassis.turnToHeading(-52, false, 12, akp, aki, akd, 3);
    spaceMaker(1);
    delay(450);
    chassis.pid(-4, -52, false, 12, 12, lkp, lki, lkd, akp, aki, akd, .5, 0);
    delay(500);
    chassis.turnToHeading(65, false, 12, akp, aki, akd, 5); // flick the ball out
    spaceMaker(0);
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
    alignerL.set(0);
    alignerR.set(1);
    pneumIntake(1);
    intakeVolts = (3.5);

    chassis.pid(200, 135, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
    delay(1600);
    intakeVolts = (2);
    chassis.turnToHeading(160, false, 12, akp*1.5, aki, akd, 0);
    spaceMaker(1);
    // matchload 23 balls
    delay(900);
    intakeVolts = (12);
    antijam = true;
    while (getRunTime() < 20) // temp values tbd if necessary
    {
        delay(10);
    }
    intakeVolts = 12;
    pneumIntake(0);
    delay(800);



    // continue in AWP or ELIM
}
