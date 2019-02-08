#ifndef _ROBOT_H_
#define _ROBOT_H_

#include "main.h"
#include "definitions.h"
#include "enums.h"
#include "drivetrain.h"
#include "forklift.h"
#include "intake.h"
#include "launcher.h"
#include "lift.h"
#include "display.h"
#include "smartController.h"

class Robot{
private:
	//Subclasses
	Forklift forklift;
	Lift lift;
	Launcher launcher;
	Intake intake;
	Drivetrain drivetrain;
	Display display;
	SmartController smartController;
public:
	//Constructors
	Robot();
	~Robot();
	//Motor target positions (encoder values)
	const int ticksRed = 1800;
	const int ticksGreen = 900;
	const int ticksBlue = 300;
	//Action log using enums (for debugging)
	std::vector<RobotActions> actionLog;
	std::vector<int> timeStamps;
	//Manual handles all methods for manual control
	bool inRange(float low, float high, float x);
	void manualControl(float leftJoy, float rightJoy);
	//Run is the method to be called in opcontrol
	//includes the display functionality, autonomous and auto distance
	void update();
};

#endif
