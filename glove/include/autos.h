#pragma once
#include "../tntnlibrary/include/api.h"

extern float startTime, endTime, totalTime;
extern tntnlib::MotorGroup intake;
extern vex::digital_out left_wing, right_wing, hang;

float getTime();
void programming_skills();
void programming_skills2();    
void awp();
void ElimAwp();
void safeAWP();
void safeElim();
