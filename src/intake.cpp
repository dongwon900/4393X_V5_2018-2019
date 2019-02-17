#include "intake.h"

void Intake::initialize(){
  intakeDirection = 1;
  intakeOn = false;
  intakeMotor.move_voltage(0);
  intakeLimitValue = intakeLimit.get_value();
  ballStaged = false;
  limitToggled = false;
}

Intake::Intake()
  :intakeMotor(INTAKE_MOTOR),
  intakeLimit(INTAKE_LIMIT_PORT)
{
  initialize();
}

Intake::~Intake() {
  intakeMotor.move_voltage(0);
}

void Intake::updateIntakeLimit(){
  intakeLimitValue = intakeLimit.get_value();
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

bool Intake::limitChangedToToggled(){
  if(intakeLimitValue == 1 && limitToggled == false){ //checks to see if the state of the limit switch has changed
    limitToggled = true;
    return true;
  }

  if(intakeLimitValue == 0 && limitToggled == true){ //checks to see if the limit has been untoggled
    limitToggled = false;
  }

  return false; //if the first statement isnt true then the limit switch didnt change therefore, return false
}

void Intake::indexerMimic(){
  if(limitChangedToToggled()){
    intakeOn = false;
  }
}

void Intake::update(){
  updateIntakeLimit();
	toggleIntake();
	intakeManualControl();
  indexerMimic();
}
