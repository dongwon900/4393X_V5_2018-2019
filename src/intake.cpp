#include "intake.h"

Intake::Intake()
  :intakeMotor(INTAKE_MOTOR),
  smartController{SmartController::instance()}
{
    intakeDirection = 1;
    intakeOn = false;
}

Intake::~Intake() {
  intakeMotor.move_voltage(0);
}

void Intake::updateController(){
  smartController = SmartController::instance();
}

void Intake::toggleIntake() {
	if(smartController.isButtonState(controllerButtonNames::B, controllerButtonState::changedToPressed)) {
    printf("%s\n", intakeOn);
		if(intakeOn) {
			intakeOn = false;
		} else {
			intakeOn = true;
		}
	}
}

void Intake::intakeManualControl(){
	if (smartController.isButtonState(controllerButtonNames::B, controllerButtonState::isPressed)){
		intakeMotor.move_voltage(-12000*intakeDirection);
	} else {
		intakeMotor.move_voltage(0);
	}
	if(intakeOn){
		intakeMotor.move_voltage(-12000*intakeDirection);
	}
}

void Intake::update(){
  updateController();
	toggleIntake();
	intakeManualControl();
}
