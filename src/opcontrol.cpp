#include "main.h"
#include "definitions.h"
#include "robot.h"

void opcontrol(){
	Robot robot;
	while (true) {
		//Everything needed for manual control
		robot.run();
		pros::delay(5);
	}
}
