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

void autonomous() {
  Robot& robot = Robot::instance();
  robot.launcher.enableLauncher();

   bool leftSide = false;
   int popSquat = (leftSide)? 9 : -9;
   int turn90 = (leftSide)? 90 : -90;
   int turn180 = (leftSide)? 180 : -180;
   int sideMod = (leftSide)? 1 : -1;

 robot.launcher.launcherMotor.move_relative(450, 200);
 robot.intake.intakeMotor.move_relative(3000, 200);

 // Left Front
 // Forward
 // Shoot high flag (+2)
 robot.drivetrain.driveDistance(36);
 robot.drivetrain.turnWithGyro(popSquat);
 robot.launcher.launcherMotor.move_relative(700, 200);
 robot.drivetrain.turnWithGyro(-popSquat);
 robot.intake.intakeMotor.move_voltage(-12000);
 // Forward into low flag (+1)
 robot.drivetrain.driveAll(8000, 8000);
 pros::Task::delay(700);
 robot.intake.intakeMotor.move_voltage(0);

 // Back up
 // Do 90 Turn to cap
 // Square off on wall
 // Move to cap
 // Flip fork up
 // Forward, hope it flips cap (+1)
 robot.drivetrain.driveDistance(-30);
 robot.drivetrain.turnWithGyro(-turn90);
 robot.drivetrain.driveAll(8000, 8000);
 pros::Task::delay(500);
 robot.drivetrain.driveAll(0, 0);
 robot.forklift.forkMotor.move_relative(2200, 200);
 robot.lift.liftMotor.move_voltage(-5000);
 robot.drivetrain.driveDistance(-18);
 robot.lift.liftMotor.move_voltage(0);
 robot.forklift.forkMotor.move_relative(2200, -200);
 pros::Task::delay(100);
 robot.drivetrain.driveDistance(-8);

 // Do 90
 // Move to platform (+3)
 robot.drivetrain.driveDistance(-8);
 robot.drivetrain.turnWithGyro(sideMod*90);
 robot.drivetrain.driveAll(-12000, -12000);
 pros::Task::delay(2500);
 robot.drivetrain.driveAll(0, 0);
}
