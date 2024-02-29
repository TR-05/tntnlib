#include "../tntnlibrary/include/screenReadout.h"

namespace tntnlib
{

void refresh_screen()
{
    Brain.Screen.setPenColor(vex::black);
    Brain.Screen.setFillColor(vex::black);
    Brain.Screen.drawRectangle(0, 0, 120, 240);
    Brain.Screen.drawRectangle(360, 0, 120, 240);
}

void screenReadout()
{
    //refresh_screen();
    Pose current(chassis.getPose(false));
    Brain.Screen.setFillColor(vex::black);
    Brain.Screen.setPenColor(vex::green);
    Brain.Screen.setFont(vex::prop30);
    std::string mode = "N/A";
    std::string state = "N/A";
    if (Competition.isDriverControl())
        mode = "Driver";
    else if (Competition.isAutonomous())
        mode = "Auto";
    if (Competition.isEnabled())
        state = "Enabled ";
    else
        state = "Disabled";
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("%s", mode.c_str());
    Brain.Screen.setCursor(2, 1);
    Brain.Screen.print("%s", state.c_str());
    Brain.Screen.setCursor(6, 1);
    Brain.Screen.print("X: %.2f", current.x);
    Brain.Screen.setCursor(7, 1);
    Brain.Screen.print("Y: %.2f", current.y);
    Brain.Screen.setCursor(8, 1);
    Brain.Screen.print("H: %.2f", current.theta);
    // right side
    Brain.Screen.setCursor(1, 25);
    Brain.Screen.print("T:%9.2f", time(false));
    Brain.Screen.setCursor(7, 25);
    Brain.Screen.print("TX: %4.2f", chassis.sensors.horizontal1 != nullptr ? chassis.sensors.horizontal1->getDistance() : 0);
    Brain.Screen.setCursor(8, 25);
    Brain.Screen.print("TY: %4.2f", chassis.sensors.vertical1 != nullptr ? chassis.sensors.vertical1->getDistance() : 0);



    Controller.Screen.clearLine(1);
    Controller.Screen.setCursor(1, 1);
    Controller.Screen.print("X%5.1f Y%5.1f H%5.1f", current.x, current.y, current.theta);
}
}  // namespace tntnlib