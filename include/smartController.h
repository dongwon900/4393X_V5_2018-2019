#ifndef _SMARTCONTROLLER_H_
#define _SMARTCONTROLLER_H_

#include "main.h"
#include "enums.h"

class SmartController{
public:
  static SmartController* inst;
  okapi::Controller controller;
  okapi::ControllerButton forkUp;
  okapi::ControllerButton forkDown;
  okapi::ControllerButton btnUp;
  okapi::ControllerButton btnDown;
  okapi::ControllerButton shootButton;
  okapi::ControllerButton autoDistanceButton;
  okapi::ControllerButton autoButton;
  okapi::ControllerButton toggleMaxSpeedButton;
  okapi::ControllerButton toggleDriveStateButton;
  okapi::ControllerButton toggleIntakeButton;
  okapi::ControllerButton miscButton;
  okapi::ControllerButton recordAutoDataButton;
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
private:
  SmartController();
public:
  void initialize();
  ~SmartController();
  controllerButtonState buttonStateFromControllerButton(ControllerButton button);
  controllerButtonState buttonStateFromButtonIndex(int buttonIndex);
  controllerButtonState evaluateButton(int buttonIndex);
  controllerButtonState updateButton(controllerButtonNames button);
  bool inRange(int low, int high, int x);
  bool vectorDataCloseEnough(std::vector<int> newData, int resAdjust);
  void saveDataToAutoLog();
  controllerButtonNames intToButtonName(int buttonIndex);
  void update();
  controllerButtonState getButtonState(controllerButtonNames button);
  float getJoystickAxis(controllerAxisNames axis);
  bool isButtonState(controllerButtonNames button, controllerButtonState state);
  void autoLogParser(std::vector<std::vector<int>>& autoData);
  controllerButtonState intToButtonState(int x);
  void autonomousUpdate(std::vector<std::vector<int>>& autoData);
  int controllerButtonStateToInt(controllerButtonState buttonState);
  std::string printButtonStates();

  static SmartController& instance();

  void operator=(const SmartController& controller);
};

#endif
/*
current issues with smart controller are that the intake will stay on instead of getting toggled
the lift will not move at all, but the liftindex gets raised to max rigth away
the launcher has a stutter in it, most likely from the updating of the button to changedToPressed during isPRessed state
the drivetrain still works perfectly
forklift works fine, maybe a slight stutter
*/
