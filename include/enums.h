#ifndef _ENUMS_H_
#define _ENUMS_H_

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

enum controllerButtonState{
  isPressed,
  notPressed,
  changedToPressed
};

enum controllerButtonNames{
	L1,
	L2,
	R1,
	R2,
	up,
	down,
	left,
	right,
	X,
	B,
	Y,
	A
};

enum controllerAxisNames{
	leftY,
	leftX,
	rightY,
	rightX
};

#endif
