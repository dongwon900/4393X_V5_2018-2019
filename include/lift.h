#ifndef _LIFT_H_
#define _LIFT_H_

#include "main.h"
#include "definitions.h"
#include "enums.h"
#include "smartController.h"

class Lift{
public:
  Motor liftMotor;
  pros::ADIPotentiometer liftPotentiometer;
  int potValue;
  int liftIndex;
	bool liftSet;
  const std::vector<int> liftPositions {1630, 2100, 2600, 2900};
public:
  Lift();
  ~Lift();
  void updatePot();
  void manualLiftControl(bool liftUpPressed, bool liftDownPressed);
  void raiseLiftIndex();
	void lowerLiftIndex();
  int upPVal(int liftIndex);
  int lowerPVal (int liftIndex);
	void updateLiftIndex(bool liftUpChangedToPressed, bool liftDownChangedToPressed);
  void updateLiftPosition(bool liftUpPressed, bool liftDownPressed);
  void update(bool liftUpPressed, bool liftDownPressed, bool liftUpChangedToPressed, bool liftDownChangedToPressed);
};

#endif
