#include "main.h"

using namespace okapi;
Controller controller;

//Motor definitions
Motor liftMotor(LIFT_MOTOR);
Motor forkMotor(FORK_MOTOR);
Motor intakeMotor(INTAKE_MOTOR);
Motor launcherMotor(LAUNCH_MOTOR);
Motor driveLeftF(DRIVETRAIN_L_F);
Motor driveLeftB(DRIVETRAIN_L_B);
Motor driveRightF(DRIVETRAIN_R_F);
Motor driveRightB(DRIVETRAIN_R_B);

// Legacy port definitions - A-H (1-8)
#define ULTRA_ECHO_PORT_LEFT 1
#define ULTRA_PING_PORT_LEFT 2
#define ULTRA_ECHO_PORT 3
#define ULTRA_PING_PORT 4
#define LIFT_POTENTIOMETER_PORT 5
#define GYRO_PORT 6
#define LIMIT_PORT 8

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
pros::ADIUltrasonic ultrasonicRight (ULTRA_ECHO_PORT, ULTRA_PING_PORT);
pros::ADIUltrasonic ultrasonicLeft (ULTRA_ECHO_PORT_LEFT, ULTRA_PING_PORT_LEFT);

//These actions can be added to a vector to determine what the robot has been doing
enum robotActions{ //UNIMPLEMENTED
	initialized,
	inAuto,
	completedInitialMoves,
	completedAuto,
	launcherCocked,
	launcherUncocked,
	toggleFork,
	raiseLift,
	lowerLift,
	fireLauncher,
	toggleIntake,
	updateSensors,
	displayBrain,
	displayController,
	adjustedDistance,
};

//ROBOT CLASS IS USED TO STORE ALL THE SENSOR DATA
class Robot{
public:
	//Constructers
	Robot();
	//Manual movement methods
	void driveLeft(int voltage);
	void driveRight(int voltage);
	void driveAll(int leftVoltage, int rightVoltage);
	void launcher();
	void forklift();
	void intake();
	void lift();
	//Other methods
	void updateSensors();
	int getVoltageIndex();
	void updateVoltageIndex(int newVoltageIndex);
	void updateDriveState();
	void displaySensorValuesOnBrain();
	void displayOptionsOnController();
	void updateSonics();
	std::vector<bool> sonicDistanceAdjust(int leftDistance, int rightDistance);
	void adjustDistance(int lefTarget, int rightTarget);
	void toggleForklift();
	void flipScoredEnemyCap();
	void toggleIntake();
private:
	//sensor readouts
	int leftSonic;
	int rightSonic;
	int gyroAngle;
	int potValue;
	int launcherLimit;
	//Motor target positions (encoder values)
	const int ticksRed = 1800;
	const int ticksGreen = 900;
	const int ticksBlue = 300;
	const int forkMoveDown = ticksGreen*13.32; //wrong not measured (used napkin math on my phone calculator and an estimated total rotation)
	const int forkToggle = forkMoveDown * .4;
	//different conditions of the robot
	bool launcherCocked;
	bool capInPossession;
	int ballsLoaded;
	bool performingAutoFunction;
	int driveState;
	int currentVoltageIndex;
	//Action log using enums (for debugging)
	std::vector<robotActions> actionLog;
};

Robot::Robot(){
	leftSonic = ultrasonicLeft.get_value();
	rightSonic = ultrasonicRight.get_value();
	gyroAngle = ultrasonicRight.get_value();
	potValue = liftPotentiometer.get_value();
	launcherLimit = launcherLimitSwitch.isPressed();
	capInPossession = false;
	performingAutoFunction = false;
	driveState = 1;
	currentVoltageIndex = 10000;

	if(launcherLimit == 1){
		launcherCocked = true;
	} else{
		launcherCocked = false;
	}

	if(pros::competition::is_connected()){
		ballsLoaded = 1;
	} else{
		ballsLoaded = 0;
	}

  std::vector<robotActions> startSet;
	startSet.push_back(initialized);
	actionLog = startSet;
}

void Robot::updateSensors(){
	leftSonic = ultrasonicLeft.get_value();
	rightSonic = ultrasonicRight.get_value();
	gyroAngle = ultrasonicRight.get_value();
	potValue = liftPotentiometer.get_value();
	launcherLimit = launcherLimitSwitch.isPressed();
}

int Robot::getVoltageIndex(){
	return currentVoltageIndex;
}

void Robot::updateVoltageIndex(int newVoltageIndex){
	currentVoltageIndex = newVoltageIndex;
}

void Robot::updateDriveState(){
	driveState = driveState * -1;
}

//Display cluster of functions
void Robot::displaySensorValuesOnBrain() {
	pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
									 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
									 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);

	pros::lcd::print(0, "LIFT PID (pros): %d", liftPotentiometer.get_value());
	pros::lcd::print(1, "Limit Switch (H): %d", launcherLimitSwitch.isPressed());
	pros::lcd::print(2, "ULTRA Left: %d", ultrasonicLeft.get_value());
	pros::lcd::print(3, "ULTRA Right: %d", ultrasonicRight.get_value());
	pros::lcd::print(4, "Gyro: ", ultrasonicRight.get_value());
}

void Robot::displayOptionsOnController() {
 controller.clear();
 controller.setText(0, 0, "Auto 1");
 controller.setText(1, 0, "Auto 2");
 controller.setText(2, 0, "Auto 3");
}

//Drive cluster of functions
void Robot::driveLeft(int voltage){
	if(voltage > currentVoltageIndex){
		driveLeftF.moveVoltage(currentVoltageIndex);
		driveLeftB.moveVoltage(currentVoltageIndex);
	} else {
		driveLeftF.moveVoltage(voltage);
		driveLeftB.moveVoltage(voltage);
	}
}

void Robot::driveRight(int voltage){
	if(voltage > currentVoltageIndex){
		driveRightF.moveVoltage(-currentVoltageIndex);
		driveRightB.moveVoltage(-currentVoltageIndex);
	} else {
		driveRightF.moveVoltage(-voltage);
		driveRightB.moveVoltage(-voltage);
	}
}

void Robot::driveAll(int leftVoltage, int rightVoltage){
	if(driveState == 1){
		driveLeft(leftVoltage);
		driveRight(rightVoltage);
	} else if(driveState == -1){
		driveLeft(-rightVoltage);
		driveRight(-leftVoltage);
	}
}

void Robot::launcher(){
	if(launcherCocked){
		if(shootButton.isPressed()){
			launcherMotor.move_voltage(12000);
		} else {
			launcherMotor.move_voltage(500);
		}
	}	else {
		launcherMotor.move_voltage(12000);
	}
}

void Robot::intake(){
	if (intakeButton.isPressed()) {
		intakeMotor.move_voltage(12000);
	} else {
		intakeMotor.move_voltage(0);
	}
}

void Robot::lift(){
	if (btnUp.changedToPressed()) {
		liftMotor.move_voltage(12000);
	} else if (btnUp.changedToReleased()){
		liftMotor.move_voltage(0);
	}
	if (btnDown.changedToPressed()) {
		liftMotor.move_voltage(-12000);
	} else if(btnDown.changedToReleased()) {
		liftMotor.move_voltage(0);
	}
}

void Robot::forklift(){
	if (forkUp.changedToPressed()) {
		forkMotor.move_voltage(12000);
	} else if (forkUp.changedToReleased()){
		forkMotor.move_voltage(0);
	}
	if (forkDown.changedToPressed()) {
		forkMotor.move_voltage(-12000);
	} else if (forkDown.changedToReleased()){
		forkMotor.move_voltage(0);
	}
}

//Sonic updating with a filter
void Robot::updateSonics(){
	if(ultrasonicLeft.get_value() != -1){
		leftSonic = ultrasonicLeft.get_value();
	}
	if(ultrasonicRight.get_value() != -1){
		rightSonic = ultrasonicRight.get_value();
	}
}

//meant to be called in a seperate while loop that way multiple 'auto' functions can be called and run at the same time
//returns a vector of bools that are true if the ultrasonic is in the correct position ([0] for left and [1] for right)
std::vector<bool> Robot::sonicDistanceAdjust(int leftDistance, int rightDistance){
	updateSonics();
	bool leftSet = false;
	bool rightSet = false;
	driveState = 1;

	//left adjustment
	if(leftDistance-10 < leftSonic && leftDistance+10 > leftSonic){
		driveLeft(0);
		leftSet = true;
	}
	else if(leftDistance-10 > leftSonic){ //going forward (with respect ot the forklift being the front) means the 'speed' inputted must be negative
		driveLeft(2000);
	}
	else if(leftDistance+10 < leftSonic){
		driveLeft(-2000);
	}

	//right adjustment
	if(rightDistance-10 < rightSonic && rightDistance+10 > rightSonic){
		driveRight(0);
		rightSet = true;
	}
	else if(rightDistance-10 > rightSonic){
		driveRight(2000);
	}
	else if(rightDistance+10 < rightSonic){
		driveRight(-2000);
	}

	return std::vector<bool>{leftSet, rightSet};
}

//AUTO FUNCTIONS THAT CAN BE USED IN AUTONOMOUS OR IN DRIVER CONTROL
//adjustDistance should be used where the moveVoltagements are sequential and not simultanous, otherwise use sonicDistanceAdjust in the parent function
void Robot::adjustDistance(int leftTarget, int rightTarget){
	bool completed = false;
	std::vector<bool> setSides;

	while(!completed){
		setSides = sonicDistanceAdjust(leftTarget, rightTarget);
		if(setSides[0] == true && setSides[1] == true){
			completed = true;
		}
	}
}

void opcontrol(){
	Robot robot;

	while (true) {
	  robot.displaySensorValuesOnBrain();
		robot.displayOptionsOnController();

		//DRIVING FUNCTIONALITY
		if (toggleMaxSpeedButton.changedToPressed()) {
			if(robot.getVoltageIndex() == 10000){
				robot.updateVoltageIndex(12000);
			} else if (robot.getVoltageIndex() == 12000){
				robot.updateVoltageIndex(10000);
			}
		}

		if (driveReverseButton.changedToPressed()) {
			robot.updateDriveState();
		}

		//GENERAL MOVEMENT
		int leftMultiplier = 127 / controller.getAnalog(ControllerAnalog::leftY); //these multipliers are to get what percent of the total voltage should be used
		int rightMultiplier = 127 / controller.getAnalog(ControllerAnalog::rightY);
		robot.driveAll(robot.getVoltageIndex()*leftMultiplier, robot.getVoltageIndex()*rightMultiplier);
		robot.intake();
		robot.launcher();
		robot.lift();
		robot.forklift();

		//EXTRA FUNCTIONALITY (not needed for normal manual operation)
		if (autoDistanceButton.changedToPressed()) {
			robot.adjustDistance(4000, 400);
		}

		if (autoButton.isPressed() && shootButton.isPressed()) {
			autonomous();
		}

		pros::delay(20);
	}

}