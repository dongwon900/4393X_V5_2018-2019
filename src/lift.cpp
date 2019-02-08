#include "lift.h"

Lift::Lift()
  :liftMotor(LIFT_MOTOR),
  liftPotentiometer(LIFT_POTENTIOMETER_PORT) {
  potValue = liftPotentiometer.get_value();
  liftIndex = 0;
  liftSet = false;
}

Lift::~Lift(){
  liftMotor.move_voltage(0);
}

void Lift::updateController(SmartController smartController){
  controller = smartController;
}

void Lift::updatePot(){
  potValue = liftPotentiometer.get_value();
}

void Lift::manualLiftControl(){
	if (controller.isButtonState(controllerButtonNames::R1, controllerButtonState::isPressed)) {
		liftMotor.move_voltage(12000);
	} else if (controller.isButtonState(controllerButtonNames::R2, controllerButtonState::isPressed)) {
		liftMotor.move_voltage(-12000);
	} else if(liftSet){
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
	if(controller.isButtonState(controllerButtonNames::R1, controllerButtonState::changedToPressed)){
		raiseLiftIndex();
	}
	if(controller.isButtonState(controllerButtonNames::R2, controllerButtonState::changedToPressed)){
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
      break;
    case 1:
      return 11000;
      break;
    case 2:
      return 12000;
      break;
    case 3:
      return 12000;
      break;
  }
}

int Lift::lowerPVal(int liftIndex){
  switch(liftIndex){
    case 0:
      return -5000;
      break;
    case 1:
      return -3000;
      break;
    case 2:
      return -2000;
      break;
    case 3:
      return -1000;
      break;
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
	} else if(!controller.isButtonState(controllerButtonNames::R1, controllerButtonState::isPressed) &&
            !controller.isButtonState(controllerButtonNames::R2, controllerButtonState::isPressed)){
		liftSet = true;
	}
}

void Lift::update(SmartController smartController){
  updateController(smartController);
  updatePot();
	updateLiftIndex();
	updateLiftPosition();
  manualLiftControl();
}

int Lift::getLiftIndex(){
  return liftIndex;
}
