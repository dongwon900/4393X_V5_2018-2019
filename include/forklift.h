#ifndef _FORKLIFT_H_
#define _FORKLIFT_H_

#include "main.h"
#include "definitions.h"
#include "enums.h"
#include "smartController.h"

class Forklift{
public:
	Motor forkMotor;
public:
	Forklift();
	~Forklift();
	void manualForkControl(bool forkUp, bool forkDown);
	void update(bool forkUp, bool forkDown);
};

#endif
