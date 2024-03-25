#pragma once
#include "../tntnlibrary/include/api.h"

extern tntnlib::MotorGroup intake;
extern vex::digital_out left_wing, right_wing, hang;
extern bool antijam;
extern float intakeVolts;
void programming_skills();
void awp();
void safeAWP();
void saferAWP();
void elim();
void safeElim();
void saferElim();
