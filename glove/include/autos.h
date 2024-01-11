#pragma once
#include "../tntnlibrary/include/api.h"

extern float startTime, endTime, totalTime;
extern tntnlib::MotorGroup intake;
extern vex::digital_out left_wing;
extern vex::digital_out right_wing;

float getTime();
void programming_skills();
void awp();
void ElimAwp();

