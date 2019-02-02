#ifndef _LAUNCHER_H_
#define _LAUNCHER_H_

#include "main.h"
#include "definitions.h"
#include "smartController.h"

//extern Motor launcherMotor;

class Launcher{
public:
  Motor launcherMotor;
  ADIButton launcherLimitSwitch;
  int launcherLimit;
  bool launcherEnabled;
public:
  Launcher();
  ~Launcher();
  void updateLimit();
  void launcherAutoMovement();
  void update();
};

#endif