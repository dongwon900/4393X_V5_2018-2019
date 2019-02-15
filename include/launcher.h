#ifndef _LAUNCHER_H_
#define _LAUNCHER_H_

#include "main.h"
#include "definitions.h"
#include "enums.h"
#include "smartController.h"

class Launcher{
private:
  Motor launcherMotor;
  ADIButton launcherLimitSwitch;
  int launcherLimit;
  bool launcherEnabled;
  SmartController& smartController;
public:
  Launcher();
  ~Launcher();
  void updateController();
  void updateLimit();
  void enableLauncher();
  void launcherAutoMovement();
  void update();
};

#endif
