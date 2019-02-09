#include "main.h"
#include "definitions.h"
#include "robot.h"


void opcontrol() {
	Robot& robot = Robot::instance();
	robot.initialize();
	while (true) {
		//Everything needed for manual control
		robot.update();
		pros::delay(5);
	}
}
