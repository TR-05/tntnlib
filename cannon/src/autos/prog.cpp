#include "autos.h"
#include "vex.h"
using namespace tntnlib;

void programming_skills()
{
    
    chassis.breakOutTimeMs = 10000;
    chassis.startAuto(34.6, 13.2, -90);
    FWrpm = matchloadRPM;
    intakeVolts = 0;
    // Score Preload
    chassis.pid(-50, -90, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
    delay(400);

    // Go to matchload
    chassis.moveTo(21.5, 19, false, 12, 12, lkp * 1.3, lki, lkd, akp * 1.5, aki, akd * 1.25, 12, 5);
    alignerL.set(1);
    alignerR.set(0);
    chassis.turnToHeading(-133, false, 12, akp, aki, akd, 12);


    if (lowerToShoot) spaceMaker(1);


    intakeVolts = (7);
    left_intake_piston.set(1);
    right_intake_piston.set(1);
    chassis.pid(100, -133, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
    delay(1000);
    chassis.autoTankVolts(-1.5, -1.5);
    delay(250);
    alignerL.set(1);
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
    chassis.moveTo(50, 44, true, 12, 12, lkp * 1.3, lki, lkd, akp * 1.5, aki, akd * 1.25, 12, 7);

    // grab that one ball and shoot it hopefully over
    intakeVolts = 4;
    pneumIntake(1);
    chassis.turnToPose(64, 41, false, 12, akp, aki, akd, 12);
    chassis.moveTo(65, 41, false, 12, 12, lkp * 1.7, lki, lkd, akp * 1.5, aki, akd * 1.25, 12, 5);

    // bezier over to the matchloader
    Path path1(64, 41, 97.3, 40.8, 103, 35.0, 120.5, 8.5, 100);
    FWrpm = 2000;
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
    delay(1450);
    chassis.autoTankVolts(-1.5, -1.5);
    delay(250);
    intakeVolts = (2);
    chassis.turnToHeading(160, false, 12, akp * 1.5, aki, akd, 0);
    if (lowerToShoot) spaceMaker(1);
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

    // escape zone and hang
    chassis.pid(-14, 135, false, 12, 12, lkp * 1.5, lki, lkd, akp, aki, akd, 12, 6);
    Path hangPath(124,26.5,  140.8,26.8,  138.5,59.0,  138.8,76.0,  100);
    chassis.follow(hangPath, true, 12, 12, lkp*3.0, lki, lkd, akp * 2, aki, akd*1.8, 12, 15, 0);
    delay(450);
    spaceMaker(1);
    chassis.LineWait(hangPath.x3, hangPath.y3, 30, 5000);
    chassis.pid(-100, -178, false, 12, 12, lkp * 1.5, lki, lkd, akp, aki, akd, 12, 0);
    delay(2000);
    chassis.stopAuto();
    intakeVolts = 0;
    FWrpm = 0;
    delay(100000);
}