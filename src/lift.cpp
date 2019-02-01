#include "lift.h"

Lift::Lift(){
  :liftMotor{LIFT_MOTOR};
  pros::ADIPotentiometer liftPotentiometer (LIFT_POTENTIOMETER_PORT);
  potValue = liftPotentiometer.get_value();
  liftIndex = 0;
  liftSet = false;
}

Lift::~Lift(){
  liftMotor.move_voltage(0);
}

void Lift::updatePot(){
  potValue = liftPotentiometer.get_value();
}

void Lift::manualLiftControl(){
	if (btnUp.isPressed()) {
		liftMotor.move_voltage(12000);
	} else if (btnDown.isPressed()) {
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
	if(btnUp.changedToPressed()){
		raiseLiftIndex();
	}
	if(btnDown.changedToPressed()){
		lowerLiftIndex();
	}

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
}

void Lift::updateLiftPosition(){
	int diff = 0;
	if(potValue > liftPositions[liftIndex] + 10){
		liftSet = false;
		diff = potValue - liftPositions[liftIndex];
		if(diff > 100){
			liftMotor.move_voltage(-4000);
		} else {
			liftMotor.move_voltage(-200);
		}
  } else if(potValue < liftPositions[liftIndex] - 10){
		liftSet = false;
		diff = liftPositions[liftIndex] - potValue;
		if(diff > 50){
			liftMotor.move_voltage(12000);
		} else {
			liftMotor.move_voltage(11000);
		}
	} else if(!btnUp.isPressed() && !btnDown.isPressed()){
		liftSet = true;
	}
}

void Robot::update(){
  updatePot();
	updateLiftIndex();
	updateLiftPosition();
  lift();
}
