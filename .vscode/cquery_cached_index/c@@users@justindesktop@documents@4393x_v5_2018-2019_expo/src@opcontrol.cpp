#include "main.h"
#include "robot.h"

void opcontrol(){
	Robot robot;

	while (true) {

		//Everything needed for manual control
		robot.aux();
		robot.manualControl(controller.getAnalog(ControllerAnalog::leftY), controller.getAnalog(ControllerAnalog::rightY));

		//EXTRA FUNCTIONALITY (not needed for normal manual operation)
		if (autoDistanceButton.isPressed()) {
			robot.adjustDistance(400, 400);
		}

		if (autoButton.isPressed() && shootButton.isPressed()) {
			autonomous();
		}

		pros::delay(15);
	}

}
