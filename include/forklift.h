#ifndef _FORKLIFT_H_
#define _FORKLIFT_H_

#include "main.h"
#include "definitions.h"
#include "smartController.h"

//extern Motor forkMotor;

class Forklift{
public:
	Motor forkMotor;
public:
	void initialize();
	Forklift();
	~Forklift();
	void manualMovement();
	void update();
	void setBrakeMode(pros::motor_brake_mode_e_t mode);
	void bringDown();
	void moveUp(int ticks);
	void moveDown(int ticks);
	void updateAuto();
};

#endif
