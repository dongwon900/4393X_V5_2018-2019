#include "forklift.h"

Forklift::Forklift()
:forkMotor(FORK_MOTOR) {

}

Forklift::~Forklift(){
	forkMotor.move_voltage(0);
}

void Forklift::updateController(SmartController controller){
  smartController = controller;
}

void Forklift::manualForkControl(){
	if(controller.isButtonState(controllerButtonNames::L1, controllerButtonState::isPressed) && controller.isButtonState(controllerButtonNames::L2, controllerButtonState::isPressed)){
		forkMotor.move_voltage(0);
	} else if (!controller.isButtonState(controllerButtonNames::L1, controllerButtonState::isPressed) && !controller.isButtonState(controllerButtonNames::L2, controllerButtonState::isPressed)){
		forkMotor.move_voltage(0);
	} else if (controller.isButtonState(controllerButtonNames::L1, controllerButtonState::isPressed)){
		forkMotor.move_voltage(12000);
  } else if (controller.isButtonState(controllerButtonNames::L2, controllerButtonState::isPressed)){
		forkMotor.move_voltage(-12000);
	}
}

void Forklift::update(SmartController smartController){
	updateController(smartController);
	manualForkControl();
}
