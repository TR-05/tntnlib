#pragma once
#include "../tntnlibrary/include/api.h"

extern tntnlib::MotorGroup intake;
extern vex::digital_out left_wing, right_wing, hang;
extern bool antijam;
extern float intakeVolts;
void wings(bool left, bool right);
void programming_skills();
void baseMatchAuto();
void poleTouchMacro();
void safeBaseMatchAuto();
void evenSaferBaseAuto();
void elimShootMacro();

void awp();
void safeAWP();
void saferAWP();
void elim();
void safeElim();
void saferElim();
