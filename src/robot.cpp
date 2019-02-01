#include "main.h"
#include "robot.h"
#include "definitions.h"

Robot::Robot(){
  std::vector<RobotActions> startSet;
	startSet.push_back(initialized);
	actionLog = startSet;
}

Robot::~Robot(){
	drive.~drive();
	lift.~lift();
	intake.~intake();
	launcher.~launcher();
	forklift.~forklift();
	Display.~Display();
}

void Robot::manualControl(float leftJoy, float rightJoy){
	float voltTarget = (float) currentVoltageIndex;
	float leftVoltage = leftJoy * voltTarget;
	float rightVoltage = rightJoy * voltTarget;
	forklift.update();
	lift.update();
	launcher.update();
	intake.update();
	drive.update(leftVoltage, rightVoltage);
}

//The function that does everything. Run this one function in the opcontrol
void Robot::update(){
  Display.update();
  manualControl(controller.getAnalog(ControllerAnalog::leftY), controller.getAnalog(ControllerAnalog::rightY));

  //EXTRA FUNCTIONALITY (not needed for normal manual operation)
  if (autoDistanceButton.isPressed()) {
    drive.adjustDistance(360, 360);
  }
	if (autoDistanceButton2.isPressed()) {
		drive.adjustDistance(150, 150);
	}

  if (autoButton.isPressed() && shootButton.isPressed()) {
    autonomous();
  }
}
