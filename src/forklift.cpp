#include "forklift.h"

void Forklift::initialize(){
	forkMotor.move_voltage(0);
}

Forklift::Forklift()
:forkMotor(FORK_MOTOR)
{
	initialize();
}

Forklift::~Forklift(){
	forkMotor.move_voltage(0);
}

void Forklift::manualMovement(){
	if(forkUp.isPressed() && forkDown.isPressed()){
		forkMotor.move_voltage(200);
	} else if (forkUp.isPressed()){
		forkMotor.move_voltage(12000);
	} else if (forkDown.isPressed()){
		forkMotor.move_voltage(-12000);
	} else if (!forkUp.isPressed() && !forkDown.isPressed()){
		forkMotor.move_voltage(200);
	}
}

void Forklift::update(){
	manualMovement();
}

void Forklift::setBrakeMode(pros::motor_brake_mode_e_t mode){
	forkMotor.set_brake_mode(mode);
}

//takes about 1.7 seconds
void Forklift::bringDown(){
	forkMotor.move_relative(-5000, 200);
}

void Forklift::moveUp(int ticks){
	forkMotor.move_relative(ticks, 200);
}

void Forklift::moveDown(int ticks){
	forkMotor.move_relative(-ticks, 200);
}
