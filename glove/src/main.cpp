#include <iostream>

#include "../tntnlibrary/include/api.h"
#include "autos.h"
#include "vex.h"
using namespace tntnlib;
vex::competition Competition;
vex::brain Brain;

vex::thread loggerThread;
vex::thread chassisThread;
vex::thread intakeThread;

/* tntnlib robot Config */
MotorGroup leftMotors(vex::gearSetting::ratio6_1, 450, -7, 8, -9, -10);
MotorGroup rightMotors(vex::gearSetting::ratio6_1, 450, 1, 2, -3, 4);
MotorGroup intake(vex::gearSetting::ratio6_1, 600, -13, 20);
/* tracking wheels and gyro */
TrackingWheel horizontal(Brain.ThreeWirePort.A, Omniwheel::NEW_275, 6.287188, 1);
TrackingWheel vertical(Brain.ThreeWirePort.C, Omniwheel::NEW_275, -0.006111, 1);
Gyro imu(12, 1.005360);
/* chassis and controllers */
ControllerSettings linearSettings(.7, 0, 3.5, 2, 2, 12);
ControllerSettings angularSettings(.35, 0.01, 2.5, 2, 15, 12);
Drivetrain drivetrain(&leftMotors, &rightMotors, 10.0, Omniwheel::NEW_275, 450, 8);
OdomSensors sensors(&vertical, nullptr, &horizontal, nullptr, &imu);
Chassis chassis(drivetrain, linearSettings, angularSettings, sensors);
/* End of tntnlib Robot Config */

vex::digital_out left_wing(Brain.ThreeWirePort.F);
vex::digital_out right_wing(Brain.ThreeWirePort.E);
vex::digital_out hang(Brain.ThreeWirePort.G);

int logger()
{
    printf("%d\n", Brain.Screen.drawImageFromFile("melman.png", 120, 0));
    while (true)
    {
        Pose current(chassis.getPose(false));
        // printf("SX: %.2f, SR: %.2f, IMU: %.2f ", chassis.sensors.horizontal1 != nullptr ? chassis.sensors.horizontal1->getDistance() : 0, chassis.sensors.vertical1 != nullptr ? chassis.sensors.vertical1->getDistance() : 0, chassis.sensors.gyro != nullptr ? chassis.sensors.gyro->rotation() : 0);
        printf("B  X: %.2f,  Y: %.2f,  H: %.2f   T: %.2f ET:%.2f\n", current.x, current.y, current.theta, getRunTime(), totalRunTime);
        screenReadout();
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
float intakeVolts = 0;
int intake_loop()
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
                intake.spinVolts(12);
            }
            else
                intake.spinVolts(intakeVolts);
        }
        else
            intake.spinVolts(intakeVolts);
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
    intakeThread.interrupt();
    intakeThread = vex::thread(intake_loop);
}

/* runs when program first starts */
void pre_auton()
{
    printf("Entered pre_auton\n");
    chassis.initialize(true, 0, 0, 0);
    resetThreads();
}

/* runs on comp switch autonomous */
void autonomous()
{
    resetThreads();

    awp();
    // safeAWP();
    // saferAWP();
    // elim();
    // safeElim();
    // saferElim();

    // programming_skills();
}

/* runs on comp switch driver */
void usercontrol()
{
    // autonomous();
    resetThreads();
    printf("Entered Driver\n");
    chassis.stateMachineOff();
    if (Controller.ButtonLeft.pressing())
    {
        vex::wait(2600, vex::msec);
        autonomous();
    }

    while (1)
    {
        antijam = false;
        updateButtons();
        if (l1.state or a.state)
            left_wing.set(1);
        else
            left_wing.set(0);

        if (l1.state or y.state)
            right_wing.set(1);
        else
            right_wing.set(0);

        if (b.state or x.state)
            hang.set(1);
        else
            hang.set(0);
        if (r1.state)
        {
            intakeVolts = 12;
        }
        else if (r2.state)
        {
            intakeVolts = -12;
        }
        else
        {
            intakeVolts = 0;
        }

        intake.setBrakeType(vex::brakeType::brake);
        // chassis.tank(Controller.Axis3.position(), Controller.Axis2.position(), 1, 0, 100, 3); // tank
        chassis.arcade(Controller.Axis3.position(), Controller.Axis4.position(), 1, 0, 100, 3); // single stick arcade
        // chassis.arcade(Controller.Axis3.position() *.12, Controller.Axis1.position() *.12, 1, 0, 100, 3); //split arcade
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
