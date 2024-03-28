#include "autos.h"
#include "vex.h"
using namespace tntnlib;


void elimsShootMacro()
{
    intakeVolts = 12;
    antijam = true;
    while (getRunTime() < 44.9)
    {
        loadMacro(1, 1100, 400);
        shotCount += 1;
    }
    pneumIntake(1);
    chassis.stopAuto();
    intakeVolts = 0;
    FWrpm = 0;
}


void awpShootAndTouchMacro()
{
    // matchload around 15 balls and exit with time to spare
    intakeVolts = 12;
    antijam = true;
    while (getRunTime() < 39.5)
    {
        loadMacro(1, 850, 300);
        shotCount += 1;
    }

    // prep zone leave
    alignerL.set(0);
    alignerR.set(0);
    chassis.setPose(Pose(131.6, 14.7, chassis.getPose().theta)); // reset to absolute coords
    chassis.pid(6, 135, false, 12, 12, lkp * 3, lki, lkd, akp * 3, aki, akd, 12, 0);
    spaceMaker(0);
    delay(500);

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
    chassis.stopAuto();
    intakeVolts = 0;
    FWrpm = 0;
}
