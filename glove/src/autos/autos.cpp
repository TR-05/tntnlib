#include "autos.h"
#include "vex.h"
using namespace tntnlib;

void baseMatchAuto();
/* Example auto move functions
 chassis.tuneOffsets(3600, akp, aki, akd, 6, 2); //Tunes odom + imu constants: DISABLE TERMINAL PRINT then jig bot to a tile, run auto, once bot stops moving tap and hold brain screen
 chassis.pid(24, 60, 60, false, 12, 12, lkp, lki, lkd, akp, aki, akd, .4, 2); //Moves forward 24 inches looking at xy point (60,60)
 chassis.pid(-24, 0, true, 12, 12, lkp, lki, lkd, akp, aki, akd, .4, 2); //Moves backward 24 inches holding heading 0,0
 chassis.SwingOnLeftToHeading(-45, 0, 12, akp*1.6, aki, akd, 1);
 chassis.moveTo(0, 20, false, 12, 12, lkp, lki, lkd, akp*1.3, aki, akd * 1.25, 12, 2);
 chassis.boomerangTo(0, 20, -180, false, 12, 12, lkp, lki, lkd, akp, aki, akd, .2, .5, 12, 2);
 chassis.turnToHeading(0, false, 12, akp, aki, akd, 1);
*/

void wings(bool left, bool right)
{
  left_wing.set(left);
  right_wing.set(right);
}

void awp()
{
  baseMatchAuto();
  poleTouchMacro();
}

void safeAWP()
{
  safeBaseMatchAuto();
  poleTouchMacro();
}

void saferAWP()
{
  evenSaferBaseAuto();
  poleTouchMacro();
}

void elim()
{
  baseMatchAuto();
  elimShootMacro();
}

void safeElim()
{
  safeBaseMatchAuto();
  elimShootMacro();
}

void saferElim()
{
  evenSaferBaseAuto();
  elimShootMacro();
}