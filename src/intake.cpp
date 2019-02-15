#include "intake.h"

void Intake::initialize(){
  intakeDirection = 1;
  intakeOn = false;
  intakeMotor.move_voltage(0);
  photoValue = photoGate.get_value();
  ballNear = false;
}

Intake::Intake()
  :intakeMotor(INTAKE_MOTOR),
  photoGate(PHOTO_GATE_PORT)
{
  initialize();
}

Intake::~Intake() {
  intakeMotor.move_voltage(0);
}

void Intake::updatePhotoGate(){
  photoValue = photoGate.get_value();
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

void Intake::indexerMimic(){
  if(!ballNear){
    if(1500 < photoValue && photoValue < 2000){ //placeholder value
      ballNear = true;
    }
  } else if(1500 < photoValue && photoValue < 2000){ //placeholder values
    intakeMotor.move_voltage(0);
    ballNear = false;
  }
}

void Intake::update(){
  updatePhotoGate();
	toggleIntake();
	intakeManualControl();
  indexerMimic();
}
