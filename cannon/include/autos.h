#pragma once
#include "../tntnlibrary/include/api.h"

extern float shotCount;
extern float FWrpm, intakeVolts;
extern float matchloadRPM;
extern bool antijam;
extern bool lowerToShoot;
extern vex::task flywheelIntakeControl;
extern tntnlib::ControllerSettings angularSettings;
extern tntnlib::MotorGroup intake;
extern tntnlib::MotorGroup flywheel;
extern vex::digital_out left_intake_piston, right_intake_piston, alignerL, alignerR, spaceMakerL, spaceMakerR;

void programming_skills();
void awp();
void elim();

void pneumIntake(bool state);
void spaceMaker(bool state);


void baseMatchAuto();
void elimsShootMacro();
void awpShootAndTouchMacro();
void singleLoadMacro();
void loadMacro(int times, int openTime, int closedTime);

