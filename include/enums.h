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
	none,
	red,
	blue,
};

enum StartingTile{
	neither,
	front,
	back,
};

#endif
