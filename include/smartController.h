#ifndef _SMARTCONTROLLER_H_
#define _SMARTCONTROLLER_H_

#include "main.h"
#include "enums.h"

class SmartController{
private:
  Controller controller;
  ControllerButton btnUp;
  ControllerButton btnDown;
  ControllerButton forkUp;
  ControllerButton forkDown;
  ControllerButton shootButton;
  ControllerButton autoDistanceButton;
  ControllerButton autoButton;
  ControllerButton driveReverseButton;
  ControllerButton toggleMaxSpeedButton;
  ControllerButton toggleIntakeButton;
  ControllerButton autoDistanceButton2;
  ControllerButton toggleDrivePolarityButton;
  //Button and joystick state storage
  float leftY;
  float leftX;
  float rightY;
  float rightX;
  controllerButtonState L1;
  controllerButtonState L2;
  controllerButtonState R1;
  controllerButtonState R2;
  controllerButtonState up;
  controllerButtonState down;
  controllerButtonState left;
  controllerButtonState right;
  controllerButtonState X;
  controllerButtonState B;
  controllerButtonState Y;
  controllerButtonState A;
public:
  SmartController();
  ~SmartController();
  void updateButton(ControllerButton button);
  void update();
  controllerButtonState getButtonState(controllerButtonNames button);
  float getJoystickAxis(controllerAxisNames axis);
  bool isButtonState(controllerButtonNames button, controllerButtonState state);
};

#endif
