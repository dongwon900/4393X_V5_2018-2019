#include "intake.h"

void Intake::initialize(){
  intakeDirection = 1;
  intakeOn = false;
  intakeMotor.move_voltage(0);
}

Intake::Intake()
  :intakeMotor(INTAKE_MOTOR) {
  initialize();
}

Intake::~Intake() {
  intakeMotor.move_voltage(0);
}

void Intake::toggleIntake() {
	if(toggleIntakeButton.changedToPressed()) {
		if(intakeOn) {
			intakeOn = false;
		} else {
			intakeOn = true;
		}
	}
}

void Intake::intakeManualControl(){
	if (toggleIntakeButton.isPressed()){
		intakeMotor.move_voltage(-12000*intakeDirection);
	} else {
		intakeMotor.move_voltage(0);
	}
	if(intakeOn){
		intakeMotor.move_voltage(-12000*intakeDirection);
	}
}

void Intake::update(){
	toggleIntake();
	intakeManualControl();
}
