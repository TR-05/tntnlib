#include "autos.h"
#include "vex.h"
using namespace tntnlib;

void baseMatchAuto()
{
    chassis.breakOutTimeMs = 10000;
    chassis.startAuto(129.9, 38.0, -90);
    antijam = true;
    FWrpm = 0;
    intakeVolts = 0;
    // Grab first triball and pass it
    Path path1(129.9, 38.0, 114.3, 38.3, 104.3, 42.5, 94, 46, 100);
    pneumIntake(1);
    intakeVolts = (8);
    chassis.follow(path1, false, 12, 12, lkp * 1.5, lki, lkd * 1.4, akp * 1.8, aki, akd, .4, 25, 4);
    FWrpm = 1800;
    pneumIntake(0);
    delay(300);
    intakeVolts = (3);
    chassis.pid(-2, -90, false, 12, 12, lkp * 1.5, lki, lkd, akp, aki, akd, 12, 1);
    chassis.turnToHeading(-195, false, 12, akp, aki, akd, 5);
    intakeVolts = (12);
    delay(800);

    // grab second ball and pass it
    FWrpm = 1800;
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
    chassis.follow(path2, true, 11, 12, lkp, lki, lkd, akp, aki, akd, 12, 9, 9);
    delay(200);
    chassis.SwingOnLeftToHeading(-230, false, 12, akp * 1.6, aki, akd, 5);
    chassis.pid(-5, -230, false, 12, 12, lkp * 1.3, lki, lkd, akp, aki, akd, 12, 3);
    spaceMaker(1);
    delay(300);
    chassis.pid(6, -230, false, 12, 12, lkp * 1.3, lki, lkd, akp, aki, akd, 12, 3);
    chassis.turnToHeading(-180, false, 12, akp, aki, akd, 6);
    spaceMaker(0);
    chassis.pid(-12, -180, false, 12, 12, lkp * 1.3, lki, lkd, akp, aki, akd, 1, 0);
    delay(650);

    // complete awp triball
    Path path3(84.1, 64.5, 90.7, 40.0, 101.7, 50.1, 114.4, 30, 100);
    FWrpm = matchloadRPM;
    pneumIntake(1);
    intakeVolts = 3;
    chassis.follow(path3, false, 12, 12, lkp * 0.8, lki, lkd*1.2, akp, aki, akd, 12, 18, 5);
    pneumIntake(0);
    if (lowerToShoot) spaceMaker(1);
    delay(200);

    // shoot passing play
    chassis.turnToHeading(158, false, 12, akp, aki, akd, 3);
    intakeVolts = 12;
    delay(600);
    spaceMaker(0);
    FWrpm = 0;
    delay(400);

    // grab red triball
    chassis.turnToHeading(-45, false, 12, akp, aki, akd, 13);
    chassis.moveTo(124.25, 21.75, true, 12, 12, lkp * 1.0, lki, lkd * 1.5, akp * 1.8, aki, akd * 1.25, 12, 3);
    chassis.turnToHeading(-45, false, 12, akp, aki, akd, 5);
    spaceMaker(1);
    delay(600);

    // score the red triball
    chassis.pid(18.5, -45, false, 12, 12, lkp*1.5, lki, lkd, akp, aki, akd, .5, 4);
    chassis.SwingOnLeftToHeading(25, false, 12, akp * 1.4, aki, akd * 1.6, 6);
    chassis.SwingOnRightToHeading(45, false, 12, akp * 1.4, aki, akd * 1.6, 6);
    delay(400);
    spaceMaker(0);
    delay(450);
    Path awpBallPush(124.25,21.75,  109.4,19.6,  104.3,9.2,  95.6,10.4,  100);
    chassis.follow(awpBallPush, true, 12, 6, lkp * 1.4, lki, lkd, akp * 0.9, aki, akd * 1.25, .2, 14, 10);
    //chassis.moveTo(98, 12.5, true, 12, 12, lkp * 1.4, lki, lkd, akp * 1.3, aki, akd * 1.25, .2, 10);
    delay(650);

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
    chassis.turnToHeading(152.5, false, 12, akp * 1.5, aki, akd, 0);

    if (lowerToShoot) spaceMaker(1);
    // matchload 12 balls
    delay(900);
    antijam = true;
    intakeVolts = 12;
    pneumIntake(0);
    delay(700);
}