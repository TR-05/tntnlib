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
    chassis.follow(path1, false, 12, 12, lkp * 1.5, lki, lkd * 1.4, akp * 1.8, aki, akd, .4, 25, 6);
    FWrpm = 1800;
    pneumIntake(0);
    delay(300);
    intakeVolts = (2);
    chassis.moveTo(100, 51, true, 12, 12, lkp * 1.5, lki, lkd * 1.5, akp * 1.3, aki, akd * 1.25, 12, 4);
    chassis.turnToHeading(-195, false, 12, akp, aki, akd, 15);
    intakeVolts = (12);
    delay(500);

    // grab second ball and pass it
    FWrpm = 1800;
    pneumIntake(1);
    chassis.turnToPose(79, 50, false, 12, akp, aki, akd, 15);
    intakeVolts = (4);
    chassis.moveTo(79, 50, false, 12, 12, lkp * 1.5, lki, lkd * 1.5, akp * 1.3, aki, akd * 1.25, 12, 4);
    pneumIntake(0);
    delay(300);
    chassis.moveTo(90, 48, true, 12, 12, lkp * 1.5, lki, lkd * 1.5, akp * 1.3, aki, akd * 1.25, 12, 4);
    intakeVolts = (0);
    chassis.turnToHeading(-195, false, 12, akp, aki, akd, 5);
    intakeVolts = (12);
    delay(500);
    FWrpm = 0;
    chassis.turnToHeading(100, false, 12, akp, aki, akd, 20);

    // grab third and forth balls and push
    Path path2(94.4, 44.1, 89.0, 44.4, 81.3, 43.5, 83, 72, 100);
    chassis.follow(path2, true, 9, 12, lkp, lki, lkd, akp, aki, akd, 12, 8, 11);
    delay(300);
    chassis.SwingOnLeftToHeading(-230, false, 12, akp * 1.6, aki, akd, 5);
    chassis.pid(-5, -230, false, 12, 12, lkp * 1.3, lki, lkd, akp, aki, akd, 12, 3.5);
    spaceMaker(1);
    delay(500);
    chassis.pid(6, -230, false, 12, 12, lkp * 1.3, lki, lkd, akp, aki, akd, 12, 3);
    chassis.turnToHeading(180, false, 12, akp, aki, akd, 15);
    spaceMaker(0);
    chassis.pid(-30, 180, false, 12, 12, lkp * 2.0, lki, lkd, akp, aki, akd, 12, 0);
    delay(800);

    // grab preload from start tile
    FWrpm = matchloadRPM;
    pneumIntake(1);
    intakeVolts = 3;
    //Path path3(84.1, 64.5, 90.7, 40.0, 101.7, 50.1, 114.4, 30, 100);
    //chassis.follow(path3, false, 12, 12, lkp * 1.0, lki, lkd*1.2, akp, aki, akd, 12, 18, 5);
    chassis.moveTo(114.4, 30, false, 12, 12, lkp * 0.9, lki, lkd * 1.5, akp * 1.3, aki, akd * 1.25, 12, 5);

    pneumIntake(0);
    if (lowerToShoot) spaceMaker(1);
    delay(200);

    // shoot passing play
    chassis.turnToHeading(158, false, 12, akp, aki, akd, 3);
    intakeVolts = 12;
    delay(450);
    if (lowerToShoot)
    {
    spaceMaker(0);
    delay(400);
    }

    FWrpm = 0;

    // grab red triball
    chassis.turnToHeading(-45, false, 12, akp, aki, akd, 15);
    chassis.moveTo(124.5, 21.5, true, 12, 12, lkp * 1.0, lki, lkd * 1.5, akp * 1.8, aki, akd * 1.25, 12, 3);
    chassis.turnToHeading(-45, false, 12, akp, aki, akd, 5);
    spaceMaker(1);
    delay(600);

    // score the red triball
    chassis.moveTo(114.4, 30, false, 12, 12, lkp * 1.5, lki, lkd * 1.5, akp * 1.3, aki, akd * 1.25, 12, 4);
    chassis.moveTo(121.7, 20, true, 12, 12, lkp * 1.5, lki, lkd * 1.5, akp * 1.3, aki, akd * 1.25, 12, 4);
    chassis.turnToHeading(70, false, 12, akp, aki, akd, 5);
    delay(300);
    spaceMaker(0);
    delay(200);
    //chassis.moveTo(121.7, 20, true, 12, 12, lkp * 1.5, lki, lkd * 1.5, akp * 1.3, aki, akd * 1.25, 12, 4);
    delay(1000);


    intakeVolts = 0;
    chassis.stopAuto();
    delay(1000000);
    //chassis.pid(18.5, -45, false, 12, 12, lkp*1.5, lki, lkd, akp, aki, akd, .5, 4);
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
    chassis.moveTo(117.75, 22.25, false, 12, 12, lkp*1.4, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 4);
    chassis.turnToHeading(135, false, 12, akp, aki, akd, 5);
    intakeVolts = (7);
    left_intake_piston.set(1);
    right_intake_piston.set(1);
    alignerL.set(1);
    alignerR.set(0);
    chassis.pid(100, 135, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 0);
    delay(1300);
    chassis.autoTankVolts(-1.5, -1.5);
    delay(250);
    if (lowerToShoot) spaceMaker(1);
    intakeVolts = (0);
    chassis.turnToHeading(152.5, false, 12, akp * 1.5, aki, akd, 0);

    if (lowerToShoot) spaceMaker(1);
    // matchload 12 balls
    delay(900);
    antijam = true;
    intakeVolts = 12;
    pneumIntake(0);
    delay(700);
}