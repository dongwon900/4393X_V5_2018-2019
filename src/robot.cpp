#include "main.h"
#include "robot.h"
#include "definitions.h"

Robot::Robot(){
  std::vector<RobotActions> startSet;
	startSet.push_back(initialized);
	actionLog = startSet;
}

Robot::~Robot(){
	drivetrain.~drivetrain();
	lift.~lift();
	intake.~intake();
	launcher.~launcher();
	forklift.~forklift();
	Display.~Display();
}

bool Robot::inRange(float low, float high, float x){
		return x < high && x > low;
}

void Robot::manualControl(float leftJoy, float rightJoy){
	float leftAnalogDrive = controller.getAnalog(ControllerAnalog::leftY);
  float rightAnalogDrive = controller.getAnalog(ControllerAnalog::rightY);
  leftAnalogDrive = (inRange(-0.05, 0.05, leftAnalogDrive)) ? 0 : leftAnalogDrive;
  rightAnalogDrive = (inRange(-0.05, 0.05, rightAnalogDrive)) ? 0 : rightAnalogDrive;

	forklift.update();
	lift.update();
	launcher.update();
	intake.update();
	drivetrain.update(leftAnalogDrive, rightAnalogDrive);
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
