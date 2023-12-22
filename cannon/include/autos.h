#pragma once
#include "../tntnlibrary/include/api.h"

extern float startTime, endTime, totalTime;
extern tntnlib::MotorGroup intake;
extern tntnlib::Flywheel flywheel;
extern vex::digital_out left_intake_piston;
extern vex::digital_out right_intake_piston;
extern vex::digital_out aligner;
extern vex::digital_out spaceMaker;

float getTime();
void programming_skills();

void singleLoadMacro();
void loadMacro(int times, int bigDelay, int smallDelay);

