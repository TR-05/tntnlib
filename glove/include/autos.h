#pragma once
#include "../tntnlibrary/include/api.h"

extern tntnlib::MotorGroup intake;
extern vex::digital_out left_wing, right_wing, hang;
extern bool antijam;
extern float intakeVolts;
void programming_skills();
void programming_skills2();    
void awp();
void ElimAwp();
void safeAWP();
void safeElim();
