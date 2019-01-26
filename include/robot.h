#include "main.h"
#include "definitions.h"
#include <vector>

//These actions can be added to a vector to determine what the robot has been doing
enum RobotActions{
	initialized,
	inAuto,
	completedInitialMoves,
	completedAuto,
	launcherCocked,
	launcherUncocked,
	toggleFork,
	raiseLift,
	lowerLift,
	fireLauncher,
	toggleIntake,
	updateSensors,
	displayBrain,
	displayController,
	adjustedDistance,
};

enum Alliance{
	red,
	blue,
};

enum StartingTile{
	front,
	back,
};

// //Motor definitions
// Motor liftMotor();
// Motor forkMotor();
// Motor intakeMotor();
// Motor launcherMotor();
// Motor driveLeftF();
// Motor driveLeftB();
// Motor driveRightF();
// Motor driveRightB();
//
// Vision visionSensor1();
// Vision visionSensor2();
// Vision visionSensor3();
// Vision visionSensor4();
// //Controller
// Controller controller;
//
// // Controller Buttons
// ControllerButton btnUp();
// ControllerButton btnDown();
// ControllerButton forkUp();
// ControllerButton forkDown();
// ControllerButton shootButton();
// ControllerButton autoDistanceButton();
// ControllerButton autoButton();
// ControllerButton driveReverseButton();
// ControllerButton toggleMaxSpeedButton();
// ControllerButton toggleIntakeButton();
//
// // Legacy Sensor Initialization
// ADIButton launcherLimitSwitch(LIMIT_PORT);
// pros::ADIGyro gyro(GYRO_PORT);
// pros::ADIPotentiometer liftPotentiometer (LIFT_POTENTIOMETER_PORT);
// pros::ADIUltrasonic ultrasonicRight (ULTRA_ECHO_PORT, ULTRA_PING_PORT);
// pros::ADIUltrasonic ultrasonicLeft (ULTRA_ECHO_PORT_LEFT, ULTRA_PING_PORT_LEFT);

//ROBOT CLASS IS USED TO STORE ALL THE SENSOR DATA
class Robot{
public:
	//Constructors
	Robot();
	//Manual control methods
	void driveLeft(int voltage);
	void driveRight(int voltage);
	void driveAll(int leftVoltage, int rightVoltage);
	void launcher();
	void forklift();
	void intake();
	void lift();
	//Updating Sensors/States/Values Methods
	void updateSensors();
	void updateDriveState();
	void displaySensorValuesOnBrain();
	void displayDataOnController();
	void updateSonics();
	void raiseLiftIndex();
	void lowerLiftIndex();
	void toggleMaxSpeed();
	void toggleDriveState();
	void updateLiftIndex();
	//Action Methods
	std::vector<bool> sonicDistanceAdjust(int leftDistance, int rightDistance);
	void adjustDistance(int lefTarget, int rightTarget);
	void toggleForklift();
	void flipScoredEnemyCap();
	void toggleIntake();
	void liftUpIndex();
	void liftDownIndex();
	//These next methods all relate to manual control
	//Subsystem Methods (For basic manual control)
	void intakeSubsystem();
	void forkliftSubsystem();
	void liftSubsystem();
	void launcherSubsystem();
	void driveSubsystem(int leftVoltage, int rightVoltage);
	//Manual handles all methods for manual control
	void manualControl(float leftJoy, float rightJoy);
	//These are the fucntions relating to choosing the autonomous
	bool chooseAlliance(); //takes a button press on the lcd emulator
	bool chooseTile(); //takes a button press on the lcd emulator
	void chooseAuto();
	//Aux relates to the displays on cortex, controller, and console
	void aux();
	void run();
private:
	//sensor readouts
	int leftSonic;
	int rightSonic;
	int gyroAngle;
	int potValue;
	int launcherLimit;
	//Motor target positions (encoder values)
  const int ticksRed = 1800;
  const int ticksGreen = 900;
  const int ticksBlue = 300;
  const int forkMoveDownFromAuto = ticksGreen*13.32; //wrong not measured (used napkin math on my phone calculator and an estimated total rotation)
  const int forkToggle = forkMoveDownFromAuto * .4; //also wrong
  const std::vector<int> liftPositions {4079, 3690, 3185, 2840};
  const std::vector<int> liftIndexes {0,1,2,3};
	//different conditions of the robot
	int liftIndex;
	bool capInPossession;
	int ballsLoaded;
	bool performingAutoFunction;
	int driveState;
	int currentVoltageIndex;
	int intakeDirection;
  bool intakeOn;
	bool isForkDown;
	Alliance alliance;
	StartingTile startingTile;
	//Action log using enums (for debugging)
	std::vector<RobotActions> actionLog;
};
