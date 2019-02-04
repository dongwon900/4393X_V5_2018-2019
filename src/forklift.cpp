#include "forklift.h"

Forklift::Forklift()
:forkMotor(FORK_MOTOR) {

}

Forklift::~Forklift(){
	forkMotor.move_voltage(0);
}

void Forklift::manualForkControl(bool forkUp, bool forkDown){
	if(forkUp && forkDown){
		forkMotor.move_voltage(0);
	} else if (!forkUp && !forkDown){
		forkMotor.move_voltage(0);
	} else if (forkUp){
		forkMotor.move_voltage(12000);
  } else if (forkDown){
		forkMotor.move_voltage(-12000);
	}
}

void Forklift::update(bool forkUp, bool forkDown){
	manualForkControl(forkUp, forkDown);
}
