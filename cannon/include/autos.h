#pragma once
#include "../tntnlibrary/include/api.h"

extern float startTime, endTime, totalTime;
extern float shotCount;
extern float FWrpm, intakeVolts;
extern bool antijam;

extern vex::task flywheelIntakeControl;
extern tntnlib::ControllerSettings angularSettings;
extern tntnlib::MotorGroup intake;
extern tntnlib::MotorGroup flywheel;
extern vex::digital_out left_intake_piston, right_intake_piston, alignerL, alignerR, spaceMakerL, spaceMakerR;


float getTime();
void programming_skills();
void programming_skills2();

void awp();
void elimAwp();
void singleLoadMacro();
void loadMacro(int times, int bigDelay, int smallDelay);

