#include "vex.h"
#include "troglib/api.h"
#include <iostream>
using namespace vex;
// A global instance of competition
competition Competition;
brain Brain;


motor ls_front = motor(PORT8, ratio6_1, true);
motor rs_front = motor(PORT8, ratio6_1, true);
motor_group leftMotors = motor_group(ls_front);
motor_group rightMotors = motor_group(rs_front);

troglib::TrackingWheel horizontal(Brain.ThreeWirePort.G, troglib::Omniwheel::NEW_275, 0.0586285, 1);
troglib::TrackingWheel vertical(Brain.ThreeWirePort.E, troglib::Omniwheel::NEW_275, -0.0164235, 1);
inertial imu = inertial(PORT1);


troglib::Drivetrain_t drivebase {&leftMotors, &rightMotors, 10, troglib::Omniwheel::NEW_4, 300, 8};
// lateral motion controller
troglib::ChassisController_t lateralController {10, 30, 1, 100, 3, 500, 20};
// angular motion controller
troglib::ChassisController_t angularController {2, 10, 1, 100, 3, 500, 20};
// sensors for odometry
troglib::OdomSensors_t sensors {&vertical, nullptr, &horizontal, nullptr, &imu};

troglib::Chassis chassis(drivebase, lateralController, angularController, sensors);


int testTask_1(){
  printf("\n\n New Task Created\n\n");
  chassis.setPose(0,0,0);
  while(1) {
    chassis.update();
    troglib::Pose current(chassis.getPose());
    printf("\n(%.2f, %.2f, %.2f)\n", sensors.horizontal1->getDistance(), sensors.vertical1->getDistance(), sensors.imu->rotation());
    printf("(%.2f, %.2f, %.2f)\n", current.x, current.y, current.theta);
    std::cout << std::flush;
    //printf("%d\n", ++i);
    vex::wait(10, vex::msec);
  }
  return 0;
}


void pre_auton(void)
{
  chassis.initialize();

  task testTask(testTask_1);
  std::cout << ""
            << "he he he ha"
            << "\n"
            << std::flush;
  troglib::Pose test(0, 0, 0);
  troglib::Pose test2(-100, 100, 200);
  test.operator+(test2);
  std::cout << "\n"
            << test.x << ", " << test.y << ", " << test.theta << std::flush;

}

void autonomous(void)
{
}

void usercontrol(void)
{
    task testTask(testTask_1);

  while (1)
  {
    wait(10.0, msec);
  }
}

int main()
{
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
  pre_auton();

  while (true)
  {
    wait(100, msec);
  }
}
