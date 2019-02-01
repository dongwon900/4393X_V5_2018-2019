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

	// Right Front
 auto launcherController = AsyncControllerFactory::posIntegrated(LAUNCH_MOTOR);

 myChassis.setMaxVoltage(3000);
 // Low flag
 myChassis.moveDistanceAsync(3.5_ft);
 myChassis.waitUntilSettled();
 // High flag
 myChassis.moveDistanceAsync(-0.2_ft);
 myChassis.waitUntilSettled();
 launcherController.setTarget(launcherMotorAuto.getPosition() + 500);
 launcherController.waitUntilSettled();
 // Ground cap
 myChassis.moveDistanceAsync(-1.5_ft);
 myChassis.waitUntilSettled();

 myChassis.turnAngleAsync(-100_deg);
 myChassis.waitUntilSettled();

 intakeMotorAuto.moveVoltage(12000);
 myChassis.moveDistanceAsync(2.5_ft);
 myChassis.waitUntilSettled();
 intakeMotorAuto.moveVoltage(0);

 myChassis.turnAngleAsync(-100_deg);
 myChassis.waitUntilSettled();

 robot.driveAll(12000, -12000);
 pros::Task::delay(2500);
}
