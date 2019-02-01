#ifndef _FORKLIFT_H_
#define _FORKLIFT_H_

#include "main.h"
#include "definitions.h"
#include "smartController.h"

//extern Motor forkMotor;

class Forklift{
private:
	Motor forkMotor;
public:
	Forklift();
	~Forklift();
	void update();
};

#endif
