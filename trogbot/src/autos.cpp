#include "autos.h"

#include "../tntnlibrary/include/drivetrain/movements/turn.h"
#include "vex.h"

using namespace tntnlib;
/* Example auto move functions
  chassis.tuneOffsets(3600, akp, aki, akd, 6, 2); //Tunes odom + imu constants: DISABLE TERMINAL PRINT then jig bot to a tile, run auto, once bot stops moving tap and hold brain screen
  chassis.pid(24, 60, 60, false, 12, 12, lkp, lki, lkd, akp, aki, akd, .4, 2); //Moves forward 24 inches looking at xy point (60,60)
  chassis.pid(-24, 0, true, 12, 12, lkp, lki, lkd, akp, aki, akd, .4, 2); //Moves backward 24 inches holding heading 0,0
  chassis.SwingOnLeftToHeading(-45, 0, 12, akp*1.6, aki, akd, 1);
  chassis.moveTo(0, 20, false, 12, 12, lkp, lki, lkd, akp*1.3, aki, akd * 1.25, 12, 2);
  chassis.boomerangTo(0, 20, -180, false, 12, 12, lkp, lki, lkd, akp, aki, akd, .2, .5, 12, 2);
  chassis.turnToHeading(0, false, 12, akp, aki, akd, 1);
*/
void programming_skills()
{
    chassis.startAuto(118.9, 25.9, -135);
    chassis.breakOutTime = 1450;
    Path path1(118.9, 25.9, 111.5, 17.9, 106.8, 16.7, 91.9, 15.6, 100);
    chassis.follow(path1, false, 12, 12, lkp, lki, lkd, akp, aki, akd, 12, 7, 6);
    chassis.breakOutTime = 4000;

    chassis.moveTo(118.9, 25.9, true, 6, 12, lkp, lki, lkd, akp * 1.3, aki, akd * 1.25, 12, 2);
    delay(1000);
    chassis.turnToHeading(-135, false, 12, akp, aki, akd, 1);
    delay(350);

    chassis.stopAuto();
}