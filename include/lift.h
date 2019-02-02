#ifndef _LIFT_H_
#define _LIFT_H_

#include "main.h"
#include "definitions.h"
#include "smartController.h"

//extern Motor liftMotor;

class Lift{
private:
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
  void manualLiftControl();
  void raiseLiftIndex();
	void lowerLiftIndex();
  int upPVal(int liftIndex);
  int lowerPVal (int liftIndex);
	void updateLiftIndex();
  void updateLiftPosition();
  void update();
};

#endif
