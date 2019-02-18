#include "main.h"
#include "definitions.h"
#include "robot.h"


void opcontrol() {
	Robot& robot = Robot::instance();
	robot.initialize();
	while (true) {
		robot.update();
		pros::delay(5);
	}
}
