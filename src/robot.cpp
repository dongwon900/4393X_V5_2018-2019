#include "robot.h"

Robot::Robot(){
  std::vector<RobotActions> startSet;
	startSet.push_back(initialized);
	actionLog = startSet;
}

Robot::~Robot(){
}

bool Robot::inRange(float low, float high, float x){
		return x < high && x > low;
}

void Robot::manualControl(float leftJoy, float rightJoy){
	float leftAnalogDrive = leftJoy;
  float rightAnalogDrive = rightJoy;
  leftAnalogDrive = (inRange(-0.02, 0.02, leftAnalogDrive)) ? 0 : leftAnalogDrive;
  rightAnalogDrive = (inRange(-0.02, 0.02, rightAnalogDrive)) ? 0 : rightAnalogDrive;

	forklift.update(smartController.isButtonState(controllerButtonNames::L1, controllerButtonState::isPressed),
    smartController.isButtonState(controllerButtonNames::L2, controllerButtonState::isPressed));
	lift.update(smartController.isButtonState(controllerButtonNames::R1, controllerButtonState::isPressed),
    smartController.isButtonState(controllerButtonNames::R2, controllerButtonState::isPressed),
    smartController.isButtonState(controllerButtonNames::R1, controllerButtonState::changedToPressed),
    smartController.isButtonState(controllerButtonNames::R2, controllerButtonState::changedToPressed));
	launcher.update(smartController.isButtonState(controllerButtonNames::A, controllerButtonState::isPressed),
    smartController.isButtonState(controllerButtonNames::A, controllerButtonState::changedToPressed));
	intake.update(smartController.isButtonState(controllerButtonNames::B, controllerButtonState::isPressed),
    smartController.isButtonState(controllerButtonNames::B, controllerButtonState::changedToPressed));
  drivetrain.update(leftAnalogDrive, rightAnalogDrive,
    smartController.isButtonState(controllerButtonNames::up, controllerButtonState::changedToPressed),
    smartController.isButtonState(controllerButtonNames::X, controllerButtonState::changedToPressed),
    smartController.isButtonState(controllerButtonNames::left, controllerButtonState::changedToPressed));

  printf("%u\n", smartController.isButtonState(controllerButtonNames::B, controllerButtonState::changedToPressed));
}

//The function that does everything. Run this one function in the opcontrol
void Robot::update(){
  smartController.update();
  display.update();
  manualControl(smartController.getJoystickAxis(controllerAxisNames::leftY), smartController.getJoystickAxis(controllerAxisNames::rightY));

  //EXTRA FUNCTIONALITY (not needed for normal manual operation)
  if (smartController.isButtonState(controllerButtonNames::down, controllerButtonState::isPressed)) {
    drivetrain.adjustDistance(360, 360, smartController.getJoystickAxis(controllerAxisNames::leftY), smartController.getJoystickAxis(controllerAxisNames::rightY));
  }
	if (smartController.isButtonState(controllerButtonNames::Y, controllerButtonState::isPressed)) {
		drivetrain.adjustDistance(150, 150, smartController.getJoystickAxis(controllerAxisNames::leftY), smartController.getJoystickAxis(controllerAxisNames::rightY));
	}

  if (smartController.isButtonState(controllerButtonNames::right, controllerButtonState::isPressed) && smartController.isButtonState(controllerButtonNames::A, controllerButtonState::isPressed)) {
    autonomous();
  }
}
