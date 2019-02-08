#ifndef _SMARTCONTROLLER_H_
#define _SMARTCONTROLLER_H_

#include "main.h"
#include "enums.h"

class SmartController{
private:
  static SmartController* inst;
  Controller controller;
  ControllerButton forkUp;
  ControllerButton forkDown;
  ControllerButton btnUp;
  ControllerButton btnDown;
  ControllerButton shootButton;
  ControllerButton autoDistanceButton;
  ControllerButton autoButton;
  ControllerButton toggleMaxSpeedButton;
  ControllerButton toggleDriveStateButton;
  ControllerButton toggleIntakeButton;
  ControllerButton miscButton;
  ControllerButton recordAutoDataButton;
  //Button and joystick state storage
  float leftY;
  float leftX;
  float rightY;
  float rightX;
  std::vector<controllerButtonState> buttonStates;
  //Stores data for use in simulating opcontrol in auto
  int startMillis;
  int currentMillis;
  bool parsedData;
  int timestampDiff;
  bool isRecording;
  std::vector<bool> isButtonChangedToPressed;
  std::vector<bool> isButtonPressed;
  //The first four indexes are the floats converted to integral form
  //the following twelve save the controllerButtonState from 0-2 with isPressed = 1 going down the enum
  //The final int is the timestamp which is the time elapsed (in milliseconds) since the smartController class was initialized
  std::vector<std::vector<int>> autoLog;
public:
  SmartController();
  ~SmartController();
  controllerButtonState buttonStatefromControllerButton(ControllerButton button);
  controllerButtonState buttonStateFromButtonIndex(int buttonIndex);
  controllerButtonState evaluateButton(int buttonIndex);
  controllerButtonState updateButton(controllerButtonNames button);
  bool inRange(int low, int high, int x);
  bool vectorDataCloseEnough(std::vector<int> newData);
  void saveDataToAutoLog();
  void update();
  controllerButtonState getButtonState(controllerButtonNames button);
  float getJoystickAxis(controllerAxisNames axis);
  bool isButtonState(controllerButtonNames button, controllerButtonState state);
  void autoLogParser(std::vector<std::vector<int>>& autoData);
  controllerButtonState intToButtonState(int x);
  void autonomousUpdate(std::vector<std::vector<int>>& autoData);
};

#endif
