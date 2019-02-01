#include "main.h"
#include "definitions.h"
#include "enums.h"
#include "drive.h"
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
	Drive drive;
	Display display;
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
	//Manual handles all methods for manual control
	void manualControl(float leftJoy, float rightJoy);
	//Run is the method to be called in opcontrol
	//includes the display functionality, autonomous and auto distance
	void update();
};