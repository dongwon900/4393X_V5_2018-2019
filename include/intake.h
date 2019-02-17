#ifndef _INTAKE_H_
#define _INTAKE_H_

#include "main.h"
#include "definitions.h"
#include "smartController.h"

//extern Motor intakeMotor;

class Intake{
public:
  Motor intakeMotor;
  int intakeDirection;
  bool intakeOn;
  int intakeLimitValue;
  bool limitToggled;
  bool ballStaged;
  pros::ADIButton intakeLimit;
public:
  void initialize();
  Intake();
  ~Intake();
  void updateIntakeLimit();
  void toggleIntake();
  void intakeManualControl();
  bool limitChangedToToggled();
  void indexerMimic();
  void update();
};

#endif
