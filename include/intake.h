#ifndef _INTAKE_H_
#define _INTAKE_H_

#include "main.h"
#include "definitions.h"
#include "smartController.h"

//extern Motor intakeMotor;

class Intake{
private:
  Motor intakeMotor;
  int intakeDirection;
  bool intakeOn;
public:
  Intake();
  ~Intake();
  void toggleIntake();
  void intakeManualControl();
  void update();
};

#endif
