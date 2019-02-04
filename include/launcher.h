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
public:
  Launcher();
  ~Launcher();
  void updateLimit();
  void enableLauncher();
  void launcherAutoMovement(bool launcherButtonIsPressed, bool launcherButtonChangedToPressed);
  void update(bool launcherButtonIsPressed, bool launcherButtonChangedToPressed);
};

#endif
