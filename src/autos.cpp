#include "autos.h"
#include "vex.h"
using namespace tntnlib;

void programming_skills()
{
  printf("Entered Auto\n");
  chassis.initialize(false, 0,0,0);
  chassis.stateMachineOn();

  /*float akp = angularController.kP;
  float aki = angularController.kI;
  float akd = angularController.kD;
  float lkp = linearController.kP;
  float lki = linearController.kI;
  float lkd = linearController.kD;

  //chassis.tuneOffsets(3600, akp, aki, akd, 6, 2); //Tunes odom + imu constants: DISABLE TERMINAL PRINT then jig bot to a tile, run auto, once bot stops moving tap and hold brain screen
  chassis.moveTo(30, 30, false, 12, 12, lkp, lki, lkd, akp, aki, akd*1.25, .3, 2);
  vex::wait(200, vex::msec);
  chassis.moveTo(0, 60, true, 12, 12, lkp, lki, lkd, akp, aki, akd*1.25, .3, 2);
  vex::wait(200, vex::msec);

  chassis.boomerangTo(0, 20, -90, false, 9, 12, lkp, lki, lkd, akp, aki, akd, .3, .5, 2);
  vex::wait(200, vex::msec);
  chassis.boomerangTo(20, 20, 90, false, 9, 12, lkp, lki, lkd, akp, aki, akd, .3, .5, 2);
  vex::wait(200, vex::msec);
  chassis.boomerangTo(0, 0, 0, false, 9, 12, lkp, lki, lkd, akp, aki, akd, .3, .5, 2);
  vex::wait(200, vex::msec);
  chassis.turnToHeading(0, false, 9, akp, aki, akd, 1);*/
}