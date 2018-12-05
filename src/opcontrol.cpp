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
const int NUM_HEIGHTS = 5;
const int lift_floor = 380;
const int lift_wall = 1000;
const int lift_low = 1200;
const int lift_high = 2000;
const int lift_top = 2600;
const int heights[NUM_HEIGHTS] = {lift_floor, lift_wall, lift_low, lift_high, lift_top};
ControllerButton btnUp(ControllerDigital::R1);
ControllerButton btnDown(ControllerDigital::R2);
ControllerButton autoFlipButton(ControllerDigital::B);
Motor liftMotor(LIFT_MOTOR);
auto liftControl = AsyncControllerFactory::posIntegrated(LIFT_MOTOR);


// SHOOTING SYSTEM
const int LAUNCH_MOTOR = 4;
ADIButton launcherLimitSwitch('H');
Motor launcherMotor(LAUNCH_MOTOR);
ControllerButton shootButton(ControllerDigital::A);

// DRIVETRAIN
auto drive = ChassisControllerFactory::create({1,2}, {9,10});

// SENSOR POSITIONING //note: between 1100 and 1250 (11 and 12.5 cm) to score 
ADIUltrasonic ultrasonic(1,2);

// AUTO
ControllerButton autoButton1(ControllerDigital::A);
ControllerButton autoButton2(ControllerDigital::left);


void displaySensorValuesOnBrain() {
	pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
									 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
									 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);

	pros::lcd::print(0, "Lift PID: %d", liftMotor.getRawPosition(NULL));
	pros::lcd::print(1, "Limit Switch (H): %d", launcherLimitSwitch.isPressed());
	// ultrasonic.get() returns 0 
	// could try ultrasnoic.controllerGet() 
	pros::lcd::print(2, "Ultrasonic: %d", ultrasonic.get());


}

void flipScoredEnemyCap() {
	// set target to current PID + X
	// FLIP
	// go back down
}

void goToWall() {
	while (ultrasonic.get() > 100) {
	// Move forward until the robot is 100 cm from a solid object
		drive.setMaxVoltage(127);
	}
	drive.setMaxVoltage(0);
}

void opcontrol() {
	// LIFT SYSTEM
	int goalHeight = 0;

	while (true) {
	  displaySensorValuesOnBrain();

		// DRIVETRAIN
	  drive.tank(controller.getAnalog(ControllerAnalog::leftY),
              -1 * controller.getAnalog(ControllerAnalog::rightY));

		// SHOOTING SYSTEM
		if (launcherLimitSwitch.isPressed()) {
  		launcherMotor.move_voltage(0);
		} else {
  		if (shootButton.isPressed()) {
				launcherMotor.move_voltage(127);
			} else  {
				launcherMotor.move_voltage(0);
			}
		}

		// LIFT SYSTEM
		if (btnUp.changedToPressed() && goalHeight < NUM_HEIGHTS - 1) {
			goalHeight++;
			liftControl.setTarget(heights[goalHeight]);
		} else if (btnDown.changedToPressed() && goalHeight > 0) {
				goalHeight--;
				liftControl.setTarget(heights[goalHeight]);
		}

		if (autoFlipButton.isPressed()) {
			flipScoredEnemyCap();
		}

		// AUTO testing
		if (autoButton1.isPressed() && autoButton2.isPressed()) {
			goToWall();
			//autonomous();
		}



		pros::delay(20);
	}
}
