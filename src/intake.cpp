#include "intake.h"

Intake::Intake()
  :intakeMotor(INTAKE_MOTOR) {
    intakeDirection = 1;
    intakeOn = false;
}

Intake::~Intake() {
  intakeMotor.move_voltage(0);
}

void Intake::toggleIntake(bool intakeButtonPressed) {
	if(intakeButtonPressed) {
		if(intakeOn) {
			intakeOn = false;
		} else {
			intakeOn = true;
		}
	}
}

void Intake::intakeManualControl(bool intakeButtonChangedToPressed){
	if (intakeButtonChangedToPressed){
		intakeMotor.move_voltage(-12000*intakeDirection);
	} else {
		intakeMotor.move_voltage(0);
	}
	if(intakeOn){
		intakeMotor.move_voltage(-12000*intakeDirection);
	}
}

void Intake::update(bool intakeButtonPressed, bool intakeButtonChangedToPressed){
	toggleIntake(intakeButtonPressed);
	intakeManualControl(intakeButtonChangedToPressed);
}
