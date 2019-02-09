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
  SmartController smartCcontroller;
public:
  Intake();
  ~Intake();
  void updateController(SmartController controller);
  void toggleIntake();
  void intakeManualControl();
  void update(SmartController controller);
};

#endif
