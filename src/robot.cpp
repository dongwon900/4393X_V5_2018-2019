#include "main.h"
#include "robot.h"
#include "definitions.h"

//Motor definitions
Motor liftMotor(LIFT_MOTOR);
Motor forkMotor(FORK_MOTOR);
Motor intakeMotor(INTAKE_MOTOR);
Motor launcherMotor(LAUNCH_MOTOR);
Motor driveLeftF(DRIVETRAIN_L_F);
Motor driveLeftB(DRIVETRAIN_L_B);
Motor driveRightF(DRIVETRAIN_R_F);
Motor driveRightB(DRIVETRAIN_R_B);

Vision visionSensor1(VISION_1, pros::E_VISION_ZERO_TOPLEFT);
Vision visionSensor2(VISION_2, pros::E_VISION_ZERO_TOPLEFT);
Vision visionSensor3(VISION_3, pros::E_VISION_ZERO_TOPLEFT);
Vision visionSensor4(VISION_4, pros::E_VISION_ZERO_TOPLEFT);

//Controller
Controller controller;

// Controller Buttons
ControllerButton btnUp(ControllerDigital::R1);
ControllerButton btnDown(ControllerDigital::R2);
ControllerButton forkUp(ControllerDigital::L1);
ControllerButton forkDown(ControllerDigital::L2);
ControllerButton shootButton(ControllerDigital::A);
ControllerButton autoDistanceButton(ControllerDigital::down);
ControllerButton autoButton(ControllerDigital::right);
ControllerButton driveReverseButton(ControllerDigital::X);
ControllerButton toggleMaxSpeedButton(ControllerDigital::up);
ControllerButton toggleIntakeButton(ControllerDigital::B);

// Legacy Sensor Initialization
ADIButton launcherLimitSwitch(LIMIT_PORT);
pros::ADIGyro gyro(GYRO_PORT);
pros::ADIPotentiometer liftPotentiometer (LIFT_POTENTIOMETER_PORT);
pros::ADIUltrasonic ultrasonicRight (ULTRA_ECHO_PORT, ULTRA_PING_PORT);
pros::ADIUltrasonic ultrasonicLeft (ULTRA_ECHO_PORT_LEFT, ULTRA_PING_PORT_LEFT);

Robot::Robot(){
	leftSonic = ultrasonicLeft.get_value();
	rightSonic = ultrasonicRight.get_value();
	gyroAngle = gyro.get_value();
	potValue = liftPotentiometer.get_value();
	launcherLimit = launcherLimitSwitch.isPressed();
	capInPossession = false;
	performingAutoFunction = false;
	driveState = 1;
	currentVoltageIndex = 10000;
	intakeDirection = 1;
	intakeOn = false;
	isForkDown = false; //should probably be an if statement chekcing the encoder value of the motor
	liftIndex = 0;

	if(pros::competition::is_connected()){
		ballsLoaded = 1;
	} else{
		ballsLoaded = 0;
	}

  std::vector<RobotActions> startSet;
	startSet.push_back(initialized);
	actionLog = startSet;
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

void Robot::updateSensors(){
	updateSonics();
	gyroAngle = gyro.get_value();
	potValue = liftPotentiometer.get_value();
	launcherLimit = launcherLimitSwitch.isPressed();
}

//All methods relating to autonomous choosing
bool Robot::chooseAlliance(){
	pros::lcd::clear();
	pros::lcd::set_text(1, "Choose an Alliance");
	pros::lcd::set_text(3, "Left for Blue");
	pros::lcd::set_text(4, "Right for Red");
	if(pros::lcd::read_buttons() == 4){
		alliance = Alliance::blue;
		return true;
	} else if (pros::lcd::read_buttons() == 2){
		//lv_disp_flush(10, 50, 10, 50, COLOR_BLACK); //(teehee) I dug deep into the files and can now print images to the touchscreen
		return false;
	} else if (pros::lcd::read_buttons() == 2){
		alliance = Alliance::red;
		return true;
	} else {
		return false;
	}
}

bool Robot::chooseTile(){
	pros::lcd::clear();
	pros::lcd::set_text(1, "Choose a Tile");
	pros::lcd::set_text(3, "Left for Front");
	pros::lcd::set_text(4, "Right for Back");
	if(pros::lcd::read_buttons() == 4){
		startingTile = StartingTile::front;
		return true;
	} else if (pros::lcd::read_buttons() == 2){
		//lv_disp_flush(10, 50, 10, 50, COLOR_BLACK); //(teehee) I dug deep into the files and can now print images to the touchscreen
		return false;
	} else if (pros::lcd::read_buttons() == 2){
		startingTile = StartingTile::back;
		return true;
	} else {
		return false;
	}
}

void Robot::chooseAuto(){
	bool allianceSelected = false;
	bool tileSelected = false;

	while(!allianceSelected){
		allianceSelected = chooseAlliance();
		pros::delay(2);
	}
	while(!tileSelected){
		tileSelected = chooseTile();
		pros::delay(2);
	}
}

//All methods relating to AUX
void Robot::displaySensorValuesOnBrain() {
	pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
									 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
									 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);

	pros::lcd::print(0, "LIFT PID (pros): %d", potValue);
	pros::lcd::print(1, "lift index: %d", liftIndex);
	pros::lcd::print(3, "Gyro: %d", gyroAngle);
	pros::lcd::print(4, "Vision 1: %d", gyroAngle);
	pros::lcd::print(5, "Vision 2: %d", gyroAngle);
	pros::lcd::print(6, "Vision 3: %d", gyroAngle);
	pros::lcd::print(7, "Vision 4: %d", gyroAngle);
}

void Robot::displayDataOnController() {
 controller.clear();
 controller.setText(0, 0, "Auto 1");
 controller.setText(1, 0, "Auto 2");
 controller.setText(2, 0, "Auto 3");
}

void Robot::aux(){
	displaySensorValuesOnBrain();
	displayDataOnController();

  printf("%s\n", "I am running");
}

//Drivesubsytem cluster of functions
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

void Robot::toggleMaxSpeed(){
	if (toggleMaxSpeedButton.changedToPressed()) {
		if(currentVoltageIndex == 10000){
			currentVoltageIndex = 12000;
		} else if (currentVoltageIndex == 12000){
			currentVoltageIndex = 10000;
		}
	}
}

void Robot::toggleDriveState(){
	if (driveReverseButton.changedToPressed()) {
		driveState = driveState * -1;
	}
}

void Robot::driveSubsystem(int leftVoltage, int rightVoltage){
	toggleMaxSpeed();
	toggleDriveState();
	driveAll(leftVoltage, rightVoltage);
}

//Intake subsystem of methods
void Robot::toggleIntake() {
	if(toggleIntakeButton.changedToPressed()){
		if(intakeOn){
			intakeOn = false;
		} else {
			intakeOn = true;
		}
	}
}

void Robot::intake(){
	if (toggleIntakeButton.isPressed()){
		intakeMotor.move_voltage(-12000*intakeDirection);
	} else {
		intakeMotor.move_voltage(0);
	}
	if(intakeOn){
		intakeMotor.move_voltage(-12000*intakeDirection);
	}
}

void Robot::intakeSubsystem(){
	toggleIntake();
	intake();
}

//Launcher subsystem methods
void Robot::launcher(){
	if(launcherLimit == 1){
		if(shootButton.isPressed()){
			launcherMotor.move_voltage(12000);
		} else {
			launcherMotor.move_voltage(500);
		}
	}	else {
		launcherMotor.move_voltage(12000);
	}
}

void Robot::launcherSubsystem(){
	launcher();
}

//Lift subsystem of methods
//This is the manual lift functionality
void Robot::lift(){
	if (btnUp.isPressed()) {
		liftMotor.move_voltage(12000);
	}
	if (btnDown.isPressed()) {
		liftMotor.move_voltage(-12000);
	}
}

void Robot::raiseLiftIndex(){
	if(liftIndex != liftPositions.size()-1){
		liftIndex++;
	}
}

void Robot::lowerLiftIndex(){
	if(liftIndex != 0){
		liftIndex--;
	}
}

void Robot::updateLiftIndex(){
	//Updates it for the button presses
	if(btnUp.changedToPressed()){
		raiseLiftIndex();
	}
	if(btnDown.changedToPressed()){
		lowerLiftIndex();
	}
	/*
	//updating for passing another lift index
	int diff = liftPositions[liftIndex] - potValue;
	//In a higher liftIndex
	if(liftIndex != liftPositions.size()-1){
		if(diff > liftPositions[liftIndex+1] - liftPositions[liftIndex] + 50){
			liftIndex++;
		}
	}
	//In a lower lifTIndex
	if(diff < liftPositions[liftIndex -1] - liftPositions[liftIndex] - 50){
		liftIndex--;
	}
	*/
}

void Robot::updateLiftPosition(){
	int diff = 0;
	if(potValue > liftPositions[liftIndex] + 15){
		diff = potValue - liftPositions[liftIndex];
		if(diff > 100){
			liftMotor.move_voltage(-8000);
		} else {
			liftMotor.move_voltage(-500);
		}
  } else if(potValue < liftPositions[liftIndex] - 15){
		diff = liftPositions[liftIndex] - potValue;
		if(diff > 100){
			liftMotor.move_voltage(12000);
		} else {
			liftMotor.move_voltage(8000);
		}
	} else if(!btnUp.isPressed() && !btnDown.isPressed()){
		liftMotor.move_voltage(400);
	}
}

void Robot::liftSubsystem(){
	lift();
	updateLiftPosition();
}

//Forklift subsystem of methods
void Robot::forklift(){
	if(forkUp.isPressed() && forkDown.isPressed()){
		forkMotor.move_voltage(0);
	} else if (forkUp.isPressed()){
		forkMotor.move_voltage(12000);
  } else if (forkDown.isPressed()){
		forkMotor.move_voltage(-12000);
	} else if (!forkUp.isPressed() && !forkDown.isPressed()){
		forkMotor.move_voltage(0);
	}
}

void Robot::forkliftSubsystem(){
	forklift();
}

void Robot::manualControl(float leftJoy, float rightJoy){
  //Driving related
	float voltTarget = (float) currentVoltageIndex;
	float leftVoltage = leftJoy * voltTarget;
	float rightVoltage = rightJoy * voltTarget;
	driveSubsystem((int) leftVoltage, (int) rightVoltage);
	intakeSubsystem();
	launcherSubsystem();
	liftSubsystem();
	forkliftSubsystem();
}

//THE FOLLOWING METHODS ARE ALL RELATED TO AUTO BEHAVIOR
//meant to be called in a seperate while loop that way multiple 'auto' functions can be called and run at the same time
//returns a vector of bools that are true if the ultrasonic is in the correct position ([0] for left and [1] for right)
//The driveRight goes to left ultrasonic because the drive direction is relative to the intake and the ultrasonic direction is relative to the forklift
std::vector<bool> Robot::sonicDistanceAdjust(int leftDistance, int rightDistance) {
	updateSonics();
	bool leftSet = false;
	bool rightSet = false;

	//Equalization
	if(leftSonic < rightSonic-15 && leftDistance > leftSonic){
		driveRight(1800);
	  driveLeft(-1800);
	} else if (rightSonic < leftSonic-15 && rightDistance > rightSonic){
		driveLeft(1800);
		driveRight(-1800);
	} else {
		//left adjustment
		if(leftDistance-10 < leftSonic && leftDistance+10 > leftSonic){
			driveRight(0);
			leftSet = true;
		}
		else if(leftDistance-10 > leftSonic){ //going forward (with respect ot the forklift being the front) means the 'speed' inputted must be negative
			driveRight(1800);
		}
		else if(leftDistance+10 < leftSonic){
			driveRight(-1800);
		}

		//right adjustment
		if(rightDistance-10 < rightSonic && rightDistance+10 > rightSonic){
			driveLeft(0);
			rightSet = true;
		}
		else if(rightDistance-10 > rightSonic){
			driveLeft(1800);
		}
		else if(rightDistance+10 < rightSonic){
			driveLeft(-1800);
		}
	}

	return std::vector<bool>{leftSet, rightSet};
}

//AUTO FUNCTIONS THAT CAN BE USED IN AUTONOMOUS OR IN DRIVER CONTROL
//adjustDistance should be used where the moveVoltagements are sequential and not simultanous, otherwise use sonicDistanceAdjust in the parent function
void Robot::adjustDistance(int leftTarget, int rightTarget){
	bool completed = false;
	std::vector<bool> setSides;

	while(!completed){
		if (controller.getAnalog(ControllerAnalog::leftY) != 0 || controller.getAnalog(ControllerAnalog::rightY) != 0) {
			completed = true; 	// User interrupt
		}
		setSides = sonicDistanceAdjust(leftTarget, rightTarget);
		if(setSides[0] == true && setSides[1] == true) {
			completed = true;
		}
		pros::delay(2);
	}
}

//The function that does everything. Run this one function in the opcontrol
void Robot::run(){
  updateSensors();
  aux();
  manualControl(controller.getAnalog(ControllerAnalog::leftY), controller.getAnalog(ControllerAnalog::rightY));

  //EXTRA FUNCTIONALITY (not needed for normal manual operation)
  if (autoDistanceButton.isPressed()) {
    adjustDistance(400, 400);
  }

  if (autoButton.isPressed() && shootButton.isPressed()) {
    autonomous();
  }
}
