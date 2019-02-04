#ifndef _INTAKE_H_
#define _INTAKE_H_

#include "main.h"
#include "definitions.h"
#include "enums.h"
#include "smartController.h"

class Intake{
private:
  Motor intakeMotor;
  int intakeDirection;
  bool intakeOn;
public:
  Intake();
  ~Intake();
  void toggleIntake(bool intakeButtonPressed);
  void intakeManualControl(bool intakeButtonChangedToPressed);
  void update(bool intakeButtonPressed, bool intakeButtonChangedToPressed);
};

#endif
