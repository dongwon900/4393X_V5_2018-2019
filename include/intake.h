#include "main.h"

extern Motor intakeMotor;

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
