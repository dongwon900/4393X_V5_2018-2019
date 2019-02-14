#include "forklift.h"

Forklift::Forklift()
:forkMotor(FORK_MOTOR) {

}

Forklift::~Forklift(){
	forkMotor.move_voltage(0);
}

void Forklift::updateController(){
  smartController = SmartController::instance();
}

void Forklift::manualForkControl(){
	if(smartController.isButtonState(controllerButtonNames::L1, controllerButtonState::isPressed) && smartController.isButtonState(controllerButtonNames::L2, controllerButtonState::isPressed)){
		forkMotor.move_voltage(0);
	} else if (!smartController.isButtonState(controllerButtonNames::L1, controllerButtonState::isPressed) && !smartController.isButtonState(controllerButtonNames::L2, controllerButtonState::isPressed)){
		forkMotor.move_voltage(0);
	} else if (smartController.isButtonState(controllerButtonNames::L1, controllerButtonState::isPressed)){
		forkMotor.move_voltage(12000);
  } else if (smartController.isButtonState(controllerButtonNames::L2, controllerButtonState::isPressed)){
		forkMotor.move_voltage(-12000);
	}
}

void Forklift::update(){
	updateController();
	manualForkControl();
}
