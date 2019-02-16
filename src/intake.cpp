#include "intake.h"

void Intake::initialize(){
  intakeDirection = 1;
  intakeOn = false;
  intakeMotor.move_voltage(0);
  startingPhotoValue = photoGate.get_value_calibrated();
  currentPhotoValue = photoGate.get_value_calibrated();
  ballNear = false;
}

Intake::Intake()
  :intakeMotor(INTAKE_MOTOR),
  photoGate(PHOTO_GATE_PORT)
{
  initialize();
  photoGate.calibrate();
}

Intake::~Intake() {
  intakeMotor.move_voltage(0);
}

void Intake::updatePhotoGate(){
  currentPhotoValue = photoGate.get_value_calibrated();
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
    if(startingPhotoValue + 40 <= currentPhotoValue){ //placeholder value
      ballNear = true;
    }
  } else if(startingPhotoValue + 10 <= currentPhotoValue ){ //placeholder values
    intakeMotor.move_voltage(0);
    ballNear = false;
    intakeOn = false;
  }
}

void Intake::update(){
  updatePhotoGate();
	toggleIntake();
	intakeManualControl();
  indexerMimic();
}
