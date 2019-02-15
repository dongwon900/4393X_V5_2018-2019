#include "drivetrain.h"

Drivetrain::Drivetrain()
:driveLeftF(DRIVETRAIN_L_F),
driveLeftB(DRIVETRAIN_L_B),
driveRightF(DRIVETRAIN_R_F),
driveRightB(DRIVETRAIN_R_B),
gyro(GYRO_PORT),
ultrasonicLeft (ULTRA_ECHO_PORT_LEFT, ULTRA_PING_PORT_LEFT),
ultrasonicRight (ULTRA_ECHO_PORT, ULTRA_PING_PORT),
smartController{SmartController::instance()}
{
  gyroAngle = gyro.get_value();
  leftSonic = ultrasonicLeft.get_value();
  rightSonic = ultrasonicRight.get_value();
  driveState = 1;
  currentVoltageIndex = 10000;
}

Drivetrain::~Drivetrain(){
  driveLeftF.move_voltage(0);
  driveLeftB.move_voltage(0);
  driveRightF.move_voltage(0);
  driveRightB.move_voltage(0);
}

void Drivetrain::updateController(){
  smartController = SmartController::instance();
}

void Drivetrain::updateGyro(){
  gyroAngle = gyro.get_value();
}

void Drivetrain::updateSonics(){
  if(ultrasonicLeft.get_value() != -1){
		leftSonic = ultrasonicLeft.get_value();
	}
	if(ultrasonicRight.get_value() != -1){
		rightSonic = ultrasonicRight.get_value();
	}
}

void Drivetrain::driveLeft(int voltage){
  if(voltage > currentVoltageIndex){
    driveLeftF.moveVoltage(currentVoltageIndex);
    driveLeftB.moveVoltage(currentVoltageIndex);
  } else {
    driveLeftF.moveVoltage(voltage);
    driveLeftB.moveVoltage(voltage);
  }
}

void Drivetrain::driveRight(int voltage){
  if(voltage > currentVoltageIndex){
    driveRightF.moveVoltage(-currentVoltageIndex);
    driveRightB.moveVoltage(-currentVoltageIndex);
  } else {
    driveRightF.moveVoltage(-voltage);
    driveRightB.moveVoltage(-voltage);
  }
}

void Drivetrain::driveAll(int leftVoltage, int rightVoltage){
  if(driveState == 1){
    driveLeft(leftVoltage);
    driveRight(rightVoltage);
  } else if(driveState == -1){
   driveLeft(-rightVoltage);
   driveRight(-leftVoltage);
  }
}

void Drivetrain::toggleMaxSpeed(){
  if (smartController.getButtonState(controllerButtonNames::up) == controllerButtonState::changedToPressed) {
    if(currentVoltageIndex == 10000){
      currentVoltageIndex = 12000;
    } else if (currentVoltageIndex == 12000){
      currentVoltageIndex = 10000;
    }
  }
}

void Drivetrain::toggleDriveState(){
  if (smartController.getButtonState(controllerButtonNames::X) == controllerButtonState::changedToPressed) {
    driveState = driveState * -1;
  }
}

bool Drivetrain::inRange(float low, float high, float x){
		return x < high && x > low;
}

//returns a vector of bools that are true if the ultrasonic is in the correct position ([0] for left and [1] for right)
//The driveRight goes to left ultrasonic because the drive direction is relative to the intake and the ultrasonic direction is relative to the forklift
std::vector<bool> Drivetrain::sonicDistanceAdjust(int leftDistance, int rightDistance) {
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
void Drivetrain::adjustDistance(int leftTarget, int rightTarget, float leftY, float rightY){
	bool completed = false;
	std::vector<bool> setSides;

  while(!completed){
		if (!inRange(-0.1, 0.1, leftY) ||
				!inRange(-0.1, 0.1, rightY)) {
			completed = true; 	// User interrupt
		}
		setSides = sonicDistanceAdjust(leftTarget, rightTarget);
		if(setSides[0] == true && setSides[1] == true) {
			completed = true;
		}
    pros::delay(2);
	}
}

void Drivetrain::update(float leftVoltage, float rightVoltage){
  updateController();
  updateGyro();
  updateSonics();
  toggleMaxSpeed();
	toggleDriveState();
  //toggleDrivePolarity(toggleDrivePolarityButton);

  float leftV =  leftVoltage * (float) currentVoltageIndex;
  float rightV = rightVoltage * (float) currentVoltageIndex;

	driveAll((int) leftV, (int) rightV);
}
