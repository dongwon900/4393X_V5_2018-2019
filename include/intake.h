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
  int photoValue;
  bool ballNear;
  pros::ADILineSensor photoGate;
public:
  void initialize();
  Intake();
  ~Intake();
  void updatePhotoGate();
  void toggleIntake();
  void intakeManualControl();
  void indexerMimic();
  void update();
};

#endif
