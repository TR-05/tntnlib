#include <iostream>

#include "../tntnlibrary/include/api.h"
#include "autos.h"
#include "vex.h"
// #include "vision.h"
using namespace tntnlib;
vex::competition Competition;
vex::brain Brain;

vex::thread loggerThread;
vex::thread chassisThread;
vex::thread flywheelIntakeThread;

vex::vision::signature SIG_1(1, 6553, 7539, 7046, -2279, -1725, -2002, 7.000, 0);
vex::vision::signature SIG_2(2, 0, 0, 0, 0, 0, 0, 3.000, 0);
vex::vision::signature SIG_3(3, 0, 0, 0, 0, 0, 0, 3.000, 0);
vex::vision::signature SIG_4(4, 0, 0, 0, 0, 0, 0, 3.000, 0);
vex::vision::signature SIG_5(5, 0, 0, 0, 0, 0, 0, 3.000, 0);
vex::vision::signature SIG_6(6, 0, 0, 0, 0, 0, 0, 3.000, 0);
vex::vision::signature SIG_7(7, 0, 0, 0, 0, 0, 0, 3.000, 0);
vex::vision vision1(vex::PORT5, 9, SIG_1, SIG_2, SIG_3, SIG_4, SIG_5, SIG_6, SIG_7);
/* tntnlib robot Config */
vex::triport Expander1(vex::PORT4);
MotorGroup leftMotors(vex::gearSetting::ratio6_1, 300, -11, -12, 13, 14);
MotorGroup rightMotors(vex::gearSetting::ratio6_1, 300, 20, 19, -18, -17);
TrackingWheel horizontal(Expander1.A, tntnlib::Omniwheel::NEW_275, 3.8, 1);
TrackingWheel vertical(Expander1.C, tntnlib::Omniwheel::NEW_275, 0.14, -1);
Gyro imu(5, 1.017725);
/* chassis and controllers (DO NOT CHANGE NAMES) */
ControllerSettings linearSettings(.6, 0, 3.5, 2, 2, 12);
ControllerSettings angularSettings(.6, 0.01, 4.5, 2, 15, 12);
Drivetrain drivetrain(&leftMotors, &rightMotors, 10.0, tntnlib::Omniwheel::NEW_4, 300, 8);
OdomSensors sensors(&vertical, nullptr, &horizontal, nullptr, &imu);
Chassis chassis(drivetrain, linearSettings, angularSettings, sensors);
// Flywheel flywheel(vex::gearSetting::ratio6_1, 3600, 11, 0, 0.0, 2, -12, 11, 20, -19);
MotorGroup flywheel(vex::gearSetting::ratio6_1, 3600, -1, 2, -9, 10);
MotorGroup intake(vex::gearSetting::ratio6_1, 600, 3, -8);
/* End of tntnlib Robot Config */

vex::digital_out left_intake_piston(Brain.ThreeWirePort.A);
vex::digital_out right_intake_piston(Brain.ThreeWirePort.B);
vex::digital_out alignerL(Brain.ThreeWirePort.E);
vex::digital_out alignerR(Brain.ThreeWirePort.F);
vex::digital_out spaceMakerL(Brain.ThreeWirePort.C);
vex::digital_out spaceMakerR(Brain.ThreeWirePort.D);
/* data logger idk where to put :/ */
int logger()
{
    while (true)
    {
        Pose current(chassis.getPose(false));
        // chassis.sensors.horizontal1 != nullptr ? chassis.sensors.horizontal1->getDistance() : 0
        // printf("SX: %.2f, SR: %.2f, IMU: %.2f \n", chassis.sensors.horizontal1 != nullptr ? chassis.sensors.horizontal1->getDistance() : 0, chassis.sensors.vertical1 != nullptr ? chassis.sensors.vertical1->getDistance() : 0, chassis.sensors.gyro != nullptr ? chassis.sensors.gyro->rotation() : 0);
        //printf("  X: %.2f,  Y: %.2f,  H: %.2f   T: %.2f ET:%.2f, V:%.2f, S:%.0f,\n", current.x, current.y, current.theta, getTime(), totalTime / 1000.0, 0.0, shotCount);
        printf("%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n", getTime(), flywheel.getRPM(), flywheel.getVolts(), 0.0, current.x, current.y, current.theta);
        std::cout << std::flush;

        Brain.Screen.clearScreen(vex::color::yellow);
        Brain.Screen.setPenColor(vex::color::black);
        Brain.Screen.setFillColor(vex::color::yellow);
        Brain.Screen.setFont(vex::fontType::prop60);
        Brain.Screen.setCursor(1, 1);
        Brain.Screen.print("X:%6.2f", current.x);
        Brain.Screen.setCursor(2, 1);
        Brain.Screen.print("Y:%6.2f", current.y);
        Brain.Screen.setCursor(3, 1);
        Brain.Screen.print("H:%6.2f", current.theta);

        vex::wait(50, vex::msec);
    }
    return 0;
}

int chassisLoop()
{
    while (true)
    {
        chassis.update();
        vex::wait(10, vex::msec);
    }
    return 0;
}
float loopTime = 200;
int loopIterater = 0;
int stallCounter = 0;
bool antijam = false;
float FWrpm = 0, intakeVolts = 0;
int flywheel_intake_loop()
{
    while (true)
    {

        if (antijam)
        {
            if (intake.getCurrent() > 1.8)
                stallCounter++;
            else
                stallCounter = 0;
            if (stallCounter > 17)
            {
                loopIterater = 0;
                stallCounter = 0;
            }
            if (loopIterater < 10)
            {
                loopIterater++;
                intake.spinVolts(-12);
            }
            else
                intake.spinVolts(intakeVolts);
        }
        else
            intake.spinVolts(intakeVolts);

        flywheel.spinRPM(FWrpm);
        vex::wait(10, vex::msec);
    }
    return 0;
}

void resetThreads()
{
    loggerThread.interrupt();
    loggerThread = vex::thread(logger);
    chassisThread.interrupt();
    chassisThread = vex::thread(chassisLoop);
    flywheelIntakeThread.interrupt();
    flywheelIntakeThread = vex::thread(flywheel_intake_loop);
}

/* runs when program first starts */
void pre_auton()
{
    printf("Entered pre_auton\n");
    chassis.initialize(true, 0, 0, 0);
    flywheel.initializeVeloController(11.75, 20, 0.075, 1, 1, 0.0, 1.0);
    resetThreads();
}

/* runs on comp switch autonomous */
void autonomous()
{
    resetThreads();
    //awp();
    programming_skills2();
    // elimAwp();
    // programming_skills();
}

void singleLoadMacro(int delay)
{
    left_intake_piston.set(0);
    right_intake_piston.set(0);
    vex::wait(delay, vex::msec);
    left_intake_piston.set(1);
    right_intake_piston.set(1);
}

void loadMacro(int times, int bigDelay, int smallDelay)
{
    for (int i = 0; i < times; i++)
    {
        singleLoadMacro(smallDelay);
        vex::wait(bigDelay, vex::msec);
    }
}

/* runs on comp switch driver */
void usercontrol()
{
    resetThreads();
    printf("Entered Driver\n");
    chassis.stateMachineOff();

    // User control code here, inside the loop
    if (Controller.ButtonLeft.pressing())
    {
        vex::wait(2400, vex::msec);
        autonomous();
    }
    while (1)
    {
        updateButtons();
        left_intake_piston.set(l1.state);
        right_intake_piston.set(l1.state);
        if (right.newPress())
        {
            alignerL.set(!alignerL);
            alignerR.set(alignerL);
        }
            
        if (l2.newPress())
        {
            spaceMakerL.set(!spaceMakerL);
            spaceMakerR.set(!spaceMakerL);
        }

        if (a.state)
            FWrpm = 3300;
        if (b.state)
            FWrpm = 2750;
        if (y.state)
            FWrpm = 2000;
        if (x.state)
            FWrpm = 0;

        intakeVolts = Controller.ButtonR1.pressing() ? 12 : Controller.ButtonR2.pressing() ? -12
                                                                                           : 0;
        chassis.tank(Controller.Axis3.position(), Controller.Axis2.position(), 1, 0, 100, 3); // tank (the best drive style)
        // printf("R: %.2f, R2:%.2f, V: %.2f, C: %.2f\n", flywheel.getRPM(), intake.getRPM(), flywheel.getVolts(), flywheel.getCurrent());
        // chassis.arcade(Controller.Axis3.position() *.12, Controller.Axis4.position() *.12, 0); //single stick arcade
        // chassis.arcade(Controller.Axis3.position() *.12, Controller.Axis1.position() *.12, 0); // split arcade
        vex::wait(10.0, vex::msec);
    }
}

/* comp switch callbacks */
int main()
{
    Competition.autonomous(autonomous);
    Competition.drivercontrol(usercontrol);
    pre_auton();

    while (true)
    {
        vex::wait(100, vex::msec);
    }
}
