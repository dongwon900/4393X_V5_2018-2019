#include "lift.h"

Lift::Lift()
  :liftMotor(LIFT_MOTOR),
  liftPotentiometer(LIFT_POTENTIOMETER_PORT),
  smartController{SmartController::instance()}
{
  potValue = liftPotentiometer.get_value();
  liftIndex = 0;
  liftSet = false;
}

Lift::~Lift(){
  liftMotor.move_voltage(0);
}

void Lift::updateController(){
  smartController = SmartController::instance();
}

void Lift::updatePot(){
  potValue = liftPotentiometer.get_value();
}

void Lift::manualLiftControl(){
	if (smartController.isButtonState(controllerButtonNames::R1, controllerButtonState::isPressed)) {
		liftMotor.move_voltage(12000);
	} else if (smartController.isButtonState(controllerButtonNames::R2, controllerButtonState::isPressed)) {
		liftMotor.move_voltage(-12000);
	} else {
		liftMotor.move_voltage(370);
	}
}

void Lift::raiseLiftIndex(){
	if(liftIndex != liftPositions.size()-1){
		liftIndex++;
	}
}

void Lift::lowerLiftIndex(){
	if(liftIndex != 0){
		liftIndex--;
	}
}

void Lift::updateLiftIndex(){
	//Updates it for the button presses
	if(smartController.isButtonState(controllerButtonNames::R1, controllerButtonState::changedToPressed)){
		raiseLiftIndex();
	}
	if(smartController.isButtonState(controllerButtonNames::R2, controllerButtonState::changedToPressed)){
		lowerLiftIndex();
	}
/*
	//updating for passing another lift index
	int diff = potValue - liftPositions[liftIndex];
	//In a higher liftIndex
	if(liftIndex != liftPositions.size()-1){
		if(diff > liftPositions[liftIndex+1] - liftPositions[liftIndex]){
			liftIndex++;
		}
	}
	//In a lower lifTIndex
  if(liftIndex != 0){
	   if(diff < liftPositions[liftIndex -1] - liftPositions[liftIndex]){
       liftIndex--;
     }
  }
  */
}

int Lift::upPVal(int liftIndex){
  switch(liftIndex){
    case 0:
      return 10000;
    case 1:
      return 11000;
    case 2:
      return 12000;
    case 3:
      return 12000;
    default:
      return 12000;
  }
}

int Lift::lowerPVal(int liftIndex){
  switch(liftIndex){
    case 0:
      return -5000;
    case 1:
      return -3000;
    case 2:
      return -2000;
    case 3:
      return -1000;
    default:
      return -3000;
  }
}

void Lift::updateLiftPosition(){
	int diff = 0;
	if(potValue > liftPositions[liftIndex] + 10){
		liftSet = false;
		diff = potValue - liftPositions[liftIndex];
		if(diff > 100){
			liftMotor.move_voltage(lowerPVal(liftIndex));
		} else {
			liftMotor.move_voltage(lowerPVal(liftIndex)/3);
		}
  } else if(potValue < liftPositions[liftIndex] - 10){
		liftSet = false;
		diff = liftPositions[liftIndex] - potValue;
		if(diff > 50){
			liftMotor.move_voltage(upPVal(liftIndex));
		} else {
			liftMotor.move_voltage(upPVal(liftIndex)-1500);
		}
	} else if(smartController.isButtonState(controllerButtonNames::R1, controllerButtonState::isPressed) &&
            smartController.isButtonState(controllerButtonNames::R2, controllerButtonState::isPressed)){
		liftSet = true;
	}
}

void Lift::update(){
  updateController();

  if(smartController.isButtonState(controllerButtonNames::R1, controllerButtonState::changedToPressed)){
    printf("%s\n", "up changed to pressed");
  }
  if(smartController.isButtonState(controllerButtonNames::R2, controllerButtonState::changedToPressed)){
    printf("%s\n", "down changed to pressed");
  }

  updatePot();
	updateLiftIndex();
	updateLiftPosition();
  manualLiftControl();
}

int Lift::getLiftIndex(){
  return liftIndex;
}

int Lift::getPotValue(){
  return potValue;
}
