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
 Motor liftMotorAuto(LIFT_MOTOR);
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
    1.0,  // Maximum linear velocity of the Chassis in m/s
    5.0,  // Maximum linear acceleration of the Chassis in m/s/s
    10.0, // Maximum linear jerk of the Chassis in m/s/s/s
    myChassis // Chassis Controller
  );

 auto launcherController = AsyncControllerFactory::posIntegrated(LAUNCH_MOTOR);
 auto forkController = AsyncControllerFactory::posIntegrated(FORK_MOTOR);

 bool leftSide = true;
 QAngle popSquat = (leftSide)? 9_deg : -9_deg;
 QAngle turn90 = (leftSide)? 100_deg : -100_deg;
 QAngle turn180 = (leftSide)? 200_deg : -200_deg;
 int sideMod = (leftSide)? 1 : -1;

 myChassis.setMaxVoltage(2300);
 launcherController.setTarget(launcherMotorAuto.getPosition() + 450);
 intakeMotorAuto.moveVoltage(-12000);
 launcherController.waitUntilSettled();
 intakeMotorAuto.moveVoltage(0);

 // Left Front
 // Forward
 // Shoot high flag (+2)
 myChassis.moveDistanceAsync(3_ft);
 myChassis.waitUntilSettled();
 myChassis.turnAngle(popSquat);
 myChassis.waitUntilSettled();
 launcherController.setTarget(launcherMotorAuto.getPosition() + 700);
 launcherController.waitUntilSettled();
 myChassis.turnAngle(-popSquat);
 myChassis.waitUntilSettled();
 intakeMotorAuto.moveVoltage(-12000);
 // Forward into low flag (+1)
 robot.drivetrain.driveAll(8000, -8000);
 pros::Task::delay(650);
 intakeMotorAuto.moveVoltage(0);

 // Back up
 // Do 90 Turn to cap
 // Move to cap
 // Flip fork up
 // Forward, hope it flips cap (+1)
 myChassis.moveDistanceAsync(-2.5_ft);
 myChassis.waitUntilSettled();
 myChassis.turnAngleAsync(-turn90);
 myChassis.waitUntilSettled();
 robot.drivetrain.driveAll(8000, -8000);
 pros::Task::delay(500);
 forkController.setTarget(forkMotorAuto.getPosition() - 2500);
 forkController.waitUntilSettled();
 liftMotorAuto.moveVoltage(-5000);
 myChassis.moveDistanceAsync(-1.5_ft);
 myChassis.waitUntilSettled();
 liftMotorAuto.moveVoltage(0);
 forkController.setTarget(forkMotorAuto.getPosition() + 2500);
 pros::Task::delay(100);
 myChassis.moveDistanceAsync(-8_in);
 myChassis.waitUntilSettled();
 forkController.waitUntilSettled();
 // Do 90
 // Move to platform (+3)
 myChassis.moveDistanceAsync(-8_in);
 myChassis.waitUntilSettled();
 myChassis.turnAngleAsync(sideMod*120_deg);
 myChassis.waitUntilSettled();

 robot.driveAll(-12000, 12000);
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
