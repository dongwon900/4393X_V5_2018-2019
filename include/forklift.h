#ifndef _FORKLIFT_H_
#define _FORKLIFT_H_

#include "main.h"
#include "definitions.h"
#include "enums.h"
#include "smartController.h"

class Forklift{
private:
	Motor forkMotor;
	SmartController controller;
public:
	Forklift();
	~Forklift();
	void updateController(SmartController controller);
	void manualForkControl();
	void update();
};

#endif
