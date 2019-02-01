#include "main.h"

extern Motor forkMotor;

class Forklift{
private:
	Motor forkMotor;
public:
	Forklift();
	~Forklift();
	void update();
};
