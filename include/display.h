#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "main.h"
#include "enums.h"
#include "smartController.h"
#include "definitions.h"
#include "lift.h"
#include "launcher.h"
#include "drivetrain.h"

class Display{
private:
  Alliance alliance;
  StartingTile startingTile;
public:
  void initialize();
  Display();
  ~Display();
  void displaySensorValuesOnBrain();
  void displayDataOnController();
  bool chooseAlliance(); //takes a button press on the lcd emulator
  bool chooseTile(); //takes a button press on the lcd emulator
  void chooseAuto();
  void update();
};

#endif
