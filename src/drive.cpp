#include "drive.h"

Drive::Drive(){
:driveLeftF{DRIVETRAIN_L_F};
:driveLeftB{DRIVETRAIN_L_B};
:driveRightF{DRIVETRAIN_R_F};
:driveRightB{DRIVETRAIN_R_B};
pros::ADIGyro gyro(GYRO_PORT);
pros::ADIUltrasonic ultrasonicRight (ULTRA_ECHO_PORT, ULTRA_PING_PORT);
pros::ADIUltrasonic ultrasonicLeft (ULTRA_ECHO_PORT_LEFT, ULTRA_PING_PORT_LEFT);
}

Drive::~Drive(){
  driveLeftF.move_voltage(0);
  driveLeftB.move_votlage(0);
  driveRightF.move_voltage(0);
  driveRightB.move_voltage(0);
}

void Drive::updateGyro(){
  gyroAngle = gyro.get_value();
}

void Drive::updateSonics(){
  if(ultrasonicLeft.get_value() != -1){
		leftSonic = ultrasonicLeft.get_value();
	}
	if(ultrasonicRight.get_value() != -1){
		rightSonic = ultrasonicRight.get_value();
	}
}

void Drive::driveLeft(int voltage){
  if(voltage > currentVoltageIndex){
    driveLeftF.moveVoltage(currentVoltageIndex);
    driveLeftB.moveVoltage(currentVoltageIndex);
  } else {
    driveLeftF.moveVoltage(voltage);
    driveLeftB.moveVoltage(voltage);
  }
}

void Drive::driveRight(int voltage){
  if(voltage > currentVoltageIndex){
    driveRightF.moveVoltage(-currentVoltageIndex);
    driveRightB.moveVoltage(-currentVoltageIndex);
  } else {
    driveRightF.moveVoltage(-voltage);
    driveRightB.moveVoltage(-voltage);
  }
}

void Drive::driveAll(int leftVoltage, int rightVoltage){
  if(driveState == 1){
    driveLeft(leftVoltage);
    driveRight(rightVoltage);
  } else if(driveState == -1){
    driveLeft(-rightVoltage);
    driveRight(-leftVoltage);
  }
}

void Drive::toggleMaxSpeed(){
  if (toggleMaxSpeedButton.changedToPressed()) {
    if(currentVoltageIndex == 10000){
      currentVoltageIndex = 12000;
    } else if (currentVoltageIndex == 12000){
      currentVoltageIndex = 10000;
    }
  }
}

void Drive::toggleDriveState(){
  if (driveReverseButton.changedToPressed()) {
    driveState = driveState * -1;
  }
}

//returns a vector of bools that are true if the ultrasonic is in the correct position ([0] for left and [1] for right)
//The driveRight goes to left ultrasonic because the drive direction is relative to the intake and the ultrasonic direction is relative to the forklift
std::vector<bool> Robot::sonicDistanceAdjust(int leftDistance, int rightDistance) {
	updateSonics();
	bool leftSet = false;
	bool rightSet = false;

	//Equalization
	if((leftSonic < rightSonic-15 && leftDistance > leftSonic) || (leftSonic < rightSonic-15 && leftDistance < leftSonic)){
		driveRight(1800);
	  driveLeft(-1800);
	} else if ((rightSonic < leftSonic-15 && rightDistance > rightSonic) || (rightSonic < leftSonic-15 && rightDistance < rightSonic)){
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

void Drive::update(int leftVoltage, int rightVoltage){
  updateGyro();
  updateSonics();
  toggleMaxSpeed();
	toggleDriveState();
	driveAll(leftVoltage, rightVoltage);
}