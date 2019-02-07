#include "forklift.h"

Forklift::Forklift()
:forkMotor(FORK_MOTOR)
{
}

Forklift::~Forklift(){
	forkMotor.move_voltage(0);
}

void Forklift::update(){
	if(forkUp.isPressed() && forkDown.isPressed()){
		forkMotor.move_voltage(0);
	} else if (forkUp.isPressed()){
		forkMotor.move_voltage(12000);
  } else if (forkDown.isPressed()){
		forkMotor.move_voltage(-12000);
	} else if (!forkUp.isPressed() && !forkDown.isPressed()){
		forkMotor.move_voltage(0);
	}
}
