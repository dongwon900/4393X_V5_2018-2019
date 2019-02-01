#include "main.h"
#include "definitions.h"
#include "robot.h"

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

 Motor launcherMotorAuto(LAUNCH_MOTOR);
 Motor intakeMotorAuto(INTAKE_MOTOR);
 Motor forkMotorAuto(FORK_MOTOR);
 ADIGyro gyroAuto(6, 1);

void autonomous() {
  Robot robot;
  auto myChassis = ChassisControllerFactory::create(
    {1, 2}, // Left motors
    {-3, -4},   // Right motors
    AbstractMotor::gearset::green, // Torque gearset
    {4.25_in, 13_in} // 4 inch wheels, 13 inch wheelbase width
  );


  auto profileController = AsyncControllerFactory::motionProfile(
    10.0,  // Maximum linear velocity of the Chassis in m/s
    20.0,  // Maximum linear acceleration of the Chassis in m/s/s
    100.0, // Maximum linear jerk of the Chassis in m/s/s/s
    myChassis // Chassis Controller
  );

 auto launcherController = AsyncControllerFactory::posIntegrated(LAUNCH_MOTOR);
 auto forkController = AsyncControllerFactory::posIntegrated(FORK_MOTOR);

 myChassis.setMaxVoltage(3000);

 // Forward
 // Shoot high flag (+2)
 myChassis.moveDistanceAsync(3_ft);
 launcherController.setTarget(launcherMotorAuto.getPosition() + 700);
 launcherController.waitUntilSettled();
 // Do 180
 // Extend fork halfway
 // Forward into low flag (+1)
 // Put fork back up
 myChassis.turnAngleAsync(180_deg);
 myChassis.waitUntilSettled();
 forkController.setTarget(forkMotorAuto.getPosition() - 500);
 forkController.waitUntilSettled();
 forkController.setTarget(forkMotorAuto.getPosition() + 500);
 forkController.waitUntilSettled();

 // Back up
 // Do 90 Turn to cap
 // Move fork down
 // Move to cap
 // Flip fork up
 // Forward, hope it flips cap (+1)
 myChassis.turnAngleAsync(100_deg);
 myChassis.waitUntilSettled();
 forkController.setTarget(forkMotorAuto.getPosition() - 700);
 forkController.waitUntilSettled();
 myChassis.moveDistanceAsync(1_ft);
 myChassis.waitUntilSettled();
 forkController.setTarget(forkMotorAuto.getPosition() + 700);
 myChassis.moveDistanceAsync(-0.5_ft);
 myChassis.waitUntilSettled();
 // Do 90
 // Move to platform (+3)
 myChassis.moveDistanceAsync(2.5_ft);
 myChassis.waitUntilSettled();
 myChassis.turnAngleAsync(100_deg);
 myChassis.waitUntilSettled();

 robot.driveAll(12000, -12000);
 pros::Task::delay(2500);




 // Low flag
 // myChassis.moveDistanceAsync(3.5_ft);
 // myChassis.waitUntilSettled();
 // // High flag
 // myChassis.moveDistanceAsync(-0.2_ft);
 // myChassis.waitUntilSettled();
 // launcherController.setTarget(launcherMotorAuto.getPosition() + 700);
 // launcherController.waitUntilSettled();
 // // Ground cap
 // myChassis.moveDistanceAsync(-1.5_ft);
 // myChassis.waitUntilSettled();
 //
 // myChassis.turnAngleAsync(-100_deg);
 // myChassis.waitUntilSettled();
 //
 // intakeMotorAuto.moveVoltage(12000);
 // myChassis.moveDistanceAsync(2.5_ft);
 // myChassis.waitUntilSettled();
 // intakeMotorAuto.moveVoltage(0);
 //
 // myChassis.turnAngleAsync(-100_deg);
 // myChassis.waitUntilSettled();
 //
 // robot.driveAll(12000, -12000);
 // pros::Task::delay(2500);
}
