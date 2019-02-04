#include "main.h"
#include "definitions.h"
#include "robot.h"

void opcontrol() {
	Robot robot;
	while (true) {
		robot.update();
		pros::delay(5);
	}
}
