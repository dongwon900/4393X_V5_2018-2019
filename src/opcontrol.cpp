#include "main.h"

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */

using namespace okapi;
Controller controller;

// LIFT SYSTEM //TODO update PID values
const int LIFT_MOTOR = 3;
const int NUM_HEIGHTS = 4;
const int height1 = 20;
const int height2 = 60;
const int height3 = 100;
const int height4 = 140;
const int heights[NUM_HEIGHTS] = {height1, height2, height3, height4};
ControllerButton btnUp(ControllerDigital::R1);
ControllerButton btnDown(ControllerDigital::R2);
auto liftControl = AsyncControllerFactory::posIntegrated(LIFT_MOTOR);

// SHOOTING SYSTEM
const int LAUNCH_MOTOR = 4;
ADIButton launcherLimitSwitch('H');
Motor launcherMotor(LAUNCH_MOTOR);
ControllerButton shootButton(ControllerDigital::A);

// DRIVETRAIN
auto drive = ChassisControllerFactory::create({1,2}, {9,10});

// AUTO
ControllerButton autoButton1(ControllerDigital::A);
ControllerButton autoButton2(ControllerDigital::left);

void opcontrol() {
	// LIFT SYSTEM
	int goalHeight = 0;

	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);

		// DRIVETRAIN
	  drive.tank(controller.getAnalog(ControllerAnalog::leftY),
              controller.getAnalog(ControllerAnalog::rightY));

		// SHOOTING SYSTEM
		// Don't power the launcher if it is all the way down
		if (launcherLimitSwitch.isPressed()) {
  		launcherMotor.move_voltage(0);
		} else {
  		if (shootButton.isPressed()) {
				launcherMotor.move_voltage(127);
			} else  {
				launcherMotor.move_voltage(0);
			}
		}

		// LIFT LOGIC
		if (btnUp.changedToPressed() && goalHeight < NUM_HEIGHTS - 1) {
			// If the goal height is not at maximum and the up button is pressed, increase the setpoint
			goalHeight++;
			liftControl.setTarget(heights[goalHeight]);
		} else if (btnDown.changedToPressed() && goalHeight > 0) {
				goalHeight--;
				liftControl.setTarget(heights[goalHeight]);
		}

		// AUTO testing
		if (autoButton1.isPressed() && autoButton2.isPressed()) {
			autonomous();
		}

		pros::delay(20);
	}
}
