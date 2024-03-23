#pragma once
#include "../tntnlibrary/include/api.h"

extern tntnlib::MotorGroup intake;
extern vex::digital_out left_wing, right_wing, hang;
extern bool antijam;
extern float intakeVolts;
void programming_skills();
void awp();
void ElimAwp();
void safeAWP();
void evenSaferBaseAuto();
void safeElim();
void saferAWP();
