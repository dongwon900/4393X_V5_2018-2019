#include "robot.h"

Robot::Robot(){
  std::vector<RobotActions> startSet;
	startSet.push_back(initialized);
	actionLog = startSet;
}

Robot::~Robot(){
}

bool Robot::inRange(float low, float high, float x){
		return x < high && x > low;
}

void Robot::manualControl(float leftJoy, float rightJoy){
	float leftAnalogDrive = controller.getAnalog(ControllerAnalog::leftY);
  float rightAnalogDrive = controller.getAnalog(ControllerAnalog::rightY);
  leftAnalogDrive = (inRange(-0.02, 0.02, leftAnalogDrive)) ? 0 : leftAnalogDrive;
  rightAnalogDrive = (inRange(-0.02, 0.02, rightAnalogDrive)) ? 0 : rightAnalogDrive;

	forklift.update();
	lift.update();
	launcher.update();
	intake.update();
  drivetrain.update(leftAnalogDrive, rightAnalogDrive);
}

//The function that does everything. Run this one function in the opcontrol
void Robot::update(){
  display.update();
  manualControl(controller.getAnalog(ControllerAnalog::leftY), controller.getAnalog(ControllerAnalog::rightY));

  //EXTRA FUNCTIONALITY (not needed for normal manual operation)
  if (autoDistanceButton.isPressed()) {
    drivetrain.adjustDistance(360, 360);
  }
	if (autoDistanceButton2.isPressed()) {
		drivetrain.adjustDistance(150, 150);
	}

  if (autoButton.isPressed() && shootButton.isPressed()) {
    autonomous();
  }
}
