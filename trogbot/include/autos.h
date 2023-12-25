#pragma once
#include "../tntnlibrary/include/api.h"

extern float startTime, endTime, totalTime;
extern vex::vision::signature SIG_1;
extern tntnlib::ControllerSettings angularSettings;
extern vex::vision vision1;
std::pair<float, float> visionPower();
float getTime();
void programming_skills();

