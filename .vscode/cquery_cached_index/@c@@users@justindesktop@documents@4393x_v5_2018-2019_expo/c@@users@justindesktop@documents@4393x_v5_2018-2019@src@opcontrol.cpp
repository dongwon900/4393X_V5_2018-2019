#include "main.h"
#include <vector>

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

// Legacy port definitions - A-H (1-8)
#define ULTRA_ECHO_PORT_LEFT 1
#define ULTRA_PING_PORT_LEFT 2
#define ULTRA_ECHO_PORT 3
#define ULTRA_PING_PORT 4
#define LIFT_POTENTIOMETER_PORT 5
#define GYRO_PORT 6
#define LIMIT_PORT 8


// Constants
const int NUM_HEIGHTS = 5;
const int lift_floor = 4095;
const int lift_wall = 3590;
const int lift_low = 3000;
const int lift_high = 2500;
const int lift_top = 2000;
const int heights[NUM_HEIGHTS] = {lift_floor, lift_wall, lift_low, lift_high, lift_top};
const int NUM_VOLTAGES = 2;
const int drive_voltage = 10000;
const int climb_voltage = 12000;
const int voltages[NUM_VOLTAGES] = {drive_voltage, climb_voltage};

// Controller Buttons
ControllerButton btnUp(ControllerDigital::R1);
ControllerButton btnDown(ControllerDigital::R2);
ControllerButton forkUp(ControllerDigital::L1);
ControllerButton forkDown(ControllerDigital::L2);
ControllerButton shootButton(ControllerDigital::A);
ControllerButton intakeButton(ControllerDigital::B);

ControllerButton autoDistanceButton(ControllerDigital::down);
ControllerButton autoButton(ControllerDigital::right);
ControllerButton driveReverseButton(ControllerDigital::X);
ControllerButton toggleMaxSpeedButton(ControllerDigital::up);

// Legacy Sensor Initialization
ADIButton launcherLimitSwitch(LIMIT_PORT);
ADIGyro gyro(GYRO_PORT);
pros::ADIPotentiometer liftPotentiometer (LIFT_POTENTIOMETER_PORT);

// Direct Motor Control Initalization
Motor liftMotor(LIFT_MOTOR);
Motor forkMotor(FORK_MOTOR);
Motor intakeMotor(INTAKE_MOTOR);
Motor launcherMotor(LAUNCH_MOTOR);
Motor leftDriveF(DRIVETRAIN_L_F);
Motor leftDriveB(DRIVETRAIN_L_B);
Motor rightDriveF(DRIVETRAIN_R_F);
Motor rightDriveB(DRIVETRAIN_R_B);

// Controller Factories
auto drive = ChassisControllerFactory::create({1,2}, {9,10});

// Flags
static bool loaded;
static int reversed = 1;
static int currentVoltageIndex = 0;

// SENSOR POSITIONING //note: between 1100 and 1250 (11 and 12.5 cm) to score
pros::ADIUltrasonic ultrasonicRight (ULTRA_ECHO_PORT, ULTRA_PING_PORT);
pros::ADIUltrasonic ultrasonicLeft (ULTRA_ECHO_PORT_LEFT, ULTRA_PING_PORT_LEFT);

void displaySensorValuesOnBrain() {
	pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
									 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
									 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);

	pros::lcd::print(0, "LIFT PID (pros): %d", liftPotentiometer.get_value());
	pros::lcd::print(1, "Limit Switch (H): %d", launcherLimitSwitch.isPressed());
	pros::lcd::print(2, "ULTRA Left: %d", ultrasonicLeft.get_value());
	pros::lcd::print(3, "ULTRA Right: %d", ultrasonicRight.get_value());
	pros::lcd::print(4, "Gyro: ", gyro.get());
}

void displayOptionsOnController() {
 controller.clear();
 controller.setText(0, 0, "Auto 1");
 controller.setText(1, 0, "Auto 2");
 controller.setText(2, 0, "Auto 3");
}

//THESE ARE MADE BECAUSE BOTH SIDES NEED TO BE CALLED ONTO INDIVIDUALLY
void leftDrive(int voltage){
	leftDriveF.move_voltage(voltage);
	leftDriveB.move_voltage(voltage);
}

void rightDrive(int voltage){
	rightDriveF.move_voltage(-voltage);
	rightDriveB.move_voltage(-voltage);
}
//The speed integer controls the voltage
void driveBothSides(int leftVoltage, int rightVoltage){
	leftDrive(leftVoltage);
	rightDrive(rightVoltage);
}
//returns the values as well as changing them in a reference, can be used either way
std::vector<int> updateSonics(std::vector<int>& values){
	std::vector<int> newValues;
	newValues.push_back(ultrasonicLeft.get_value());
	newValues.push_back(ultrasonicRight.get_value());
	//passes the values through a basic filter (where -1 is undefined)
	//this also figures out which operation to use when putting the value into the vector
	if(newValues[0] != -1){
		if(values.size() >= 2){
			values[0] = newValues[0];
		}
		else{
			values.clear();
			values.push_back(newValues[0]);
		}
	}
	if(newValues[1] != -1){
		if(values.size() >= 2){
			values[1] = newValues[1];
		}
		else{
			values.push_back(newValues[1]);
		}
	}
  return values;
}

//meant to be called in a seperate while loop that way multiple 'auto' functions can be called and run at the same time
//returns a vector of bools that are true if the ultrasonic is in the correct position ([0] for left and [1] for right)
std::vector<bool> sonicDistanceAdjust(int leftLowerBound, int leftUpperBound, int rightLowerBound, int rightUpperBound, int voltage){
	bool leftSet = false;
	bool rightSet = false;
	std::vector<int> sonicValues;
	updateSonics(sonicValues);

	//left adjustment
	if(leftLowerBound < sonicValues[0] && leftUpperBound > sonicValues[0]){
		leftDrive(0);
		leftSet = true;
	}
	else if(leftLowerBound > sonicValues[0]){ //going forward (with respect ot the forklift being the front) means the 'speed' inputted must be negative
		leftDrive(voltage);
	}
	else if(leftUpperBound < sonicValues[0]){
		leftDrive(-voltage);
	}

	//right adjustment
	if(rightLowerBound < sonicValues[1] && rightUpperBound > sonicValues[1]){
		rightDrive(0);
		rightSet = true;
	}
	else if(rightLowerBound > sonicValues[1]){
		rightDrive(voltage);
	}
	else if(rightUpperBound < sonicValues[1]){
		rightDrive(-voltage);
	}

	return std::vector<bool>{leftSet, rightSet};
}

//AUTO FUNCTIONS THAT CAN BE USED IN AUTONOMOUS OR IN DRIVER CONTROL
//adjustDistance should be used where the movements are sequential and not simultanous, otherwise use sonicDistanceAdjust in the parent function
void adjustDistance(int leftTarget, int rightTarget){
	bool completed = false;
	std::vector<bool> setSides;

	while(!completed){
		setSides = sonicDistanceAdjust(leftTarget-10,leftTarget+10,rightTarget-10,rightTarget+10,2000);
		if(setSides[0] == true && setSides[1] == true){
			completed = true;
		}
	}
}

void goToWall(int distance){
	adjustDistance(distance, distance);
}

//takes true for fork down, and false for forkup
void toggleForklift(bool forkPosition){
	if(forkPosition){
		forkMotor.moveRelative(50, 200);
	}
	else{
		forkMotor.moveRelative(50, -200);
	} //super wack and not correct
}

void flipScoredEnemyCap() {
	// set target to current PID + X
	// FLIP
	// go back down
}

void opcontrol() {
	// LIFT SYSTEM
	int goalHeight = 0;

	drive.setMaxVelocity(200);
	liftMotor.setVoltageLimit(12000);

	while (true) {
	  displaySensorValuesOnBrain();
		displayOptionsOnController();

		/*
		* TO DO:
		*  - Debug the speed toggling (currently, a one-shot to max speed for endgame)
		*/

		// SPEED TOGGLING
		// if (toggleMaxSpeedButton.changedToPressed()) {
		// 	drive.setMaxVoltage(voltages[(currentVoltageIndex+1)%  NUM_VOLTAGES]);
		// 	pros::lcd::print(5, "Drivketrain Voltage: ", voltages[(currentVoltageIndex+1)%  NUM_VOLTAGES]);
		// }

		if (toggleMaxSpeedButton.changedToPressed()) {
			drive.setMaxVelocity(200);
		}

		// DRIVETRAIN
		if (driveReverseButton.changedToPressed()) {
			reversed = reversed * -1;
		}

		if (reversed == 1) {
	  drive.tank(controller.getAnalog(ControllerAnalog::leftY),
            -1 * controller.getAnalog(ControllerAnalog::rightY));
		} else if (reversed == -1) {
			drive.tank(reversed*controller.getAnalog(ControllerAnalog::rightY),
							-1*reversed*controller.getAnalog(ControllerAnalog::leftY));
		}

		//LAUNCHER SYSTEM
		loaded = launcherLimitSwitch.isPressed();
		if (loaded) {
			if (shootButton.isPressed()) {
				launcherMotor.move_voltage(12000);
			} else {
				launcherMotor.move_voltage(500);
			}
		} else {
			launcherMotor.move_voltage(12000);
		}

		//LIFT SYSTEM (Auto levelling)
		// if (btnUp.changedToPressed() && goalHeight < NUM_HEIGHTS - 1) {
		// 	goalHeight++;
		// 	liftControlPot.setTarget(heights[goalHeight]);
		// } else if (btnDown.changedToPressed() && goalHeight > 0) {
		// 		goalHeight--;
		// 		liftControlPot.setTarget(heights[goalHeight]);
		// }

		// DIRECT LIFT CONTROL
		if (btnUp.changedToPressed()) {
			liftMotor.move_velocity(200);
		} else if (btnUp.changedToReleased()){
			liftMotor.move_velocity(0);
		}
		if (btnDown.changedToPressed()) {
			liftMotor.move_velocity(-200);
		} else if(btnDown.changedToReleased()) {
			liftMotor.move_velocity(0);
		}

		// INTAKE CONTROL
		if (intakeButton.isPressed()) {
			intakeMotor.move_velocity(200);
		} else {
			intakeMotor.move_velocity(0);
		}

		// FORK CONTROl
		if (forkUp.changedToPressed()) {
			forkMotor.move_velocity(200);
		} else if (forkUp.changedToReleased()){
			forkMotor.move_velocity(0);
		}

		if (forkDown.changedToPressed()) {
			forkMotor.move_velocity(-200);
		} else if (forkDown.changedToReleased()) {
			forkMotor.move_velocity(0);
		}

		if (autoDistanceButton.changedToPressed()) {
			goToWall(400);
		}

		// AUTO testing
		if (autoButton.isPressed() && shootButton.isPressed()) {
			autonomous();
		}

		pros::delay(20);
	}

}
