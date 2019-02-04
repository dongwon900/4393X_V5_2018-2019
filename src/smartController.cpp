#include "smartController.h"

SmartController::SmartController()
:controller(),
btnUp(ControllerDigital::R1, false),
btnDown(ControllerDigital::R2, false),
forkUp(ControllerDigital::L1, false),
forkDown(ControllerDigital::L2, false),
shootButton(ControllerDigital::up, false),
autoDistanceButton(ControllerDigital::down, false),
autoButton(ControllerDigital::left, false),
driveReverseButton(ControllerDigital::right, false),
toggleMaxSpeedButton(ControllerDigital::X, false),
toggleIntakeButton(ControllerDigital::B, false),
autoDistanceButton2(ControllerDigital::Y, false),
toggleDrivePolarityButton(ControllerDigital::A, false)
{
  leftY = 0;
  leftX = 0;
  rightY = 0;
  rightX = 0;
  L1 = controllerButtonState::notPressed;
  L2 = controllerButtonState::notPressed;
  R1 = controllerButtonState::notPressed;
  R2 = controllerButtonState::notPressed;
  up = controllerButtonState::notPressed;
  down = controllerButtonState::notPressed;
  left = controllerButtonState::notPressed;
  right = controllerButtonState::notPressed;
  X = controllerButtonState::notPressed;
  B = controllerButtonState::notPressed;
  Y = controllerButtonState::notPressed;
  A = controllerButtonState::notPressed;
}

SmartController::~SmartController(){
  leftY = 0;
  leftX = 0;
  rightY = 0;
  rightX = 0;
  L1 = controllerButtonState::notPressed;
  L2 = controllerButtonState::notPressed;
  R1 = controllerButtonState::notPressed;
  R2 = controllerButtonState::notPressed;
  up = controllerButtonState::notPressed;
  down = controllerButtonState::notPressed;
  left = controllerButtonState::notPressed;
  right = controllerButtonState::notPressed;
  X = controllerButtonState::notPressed;
  B = controllerButtonState::notPressed;
  Y = controllerButtonState::notPressed;
  A = controllerButtonState::notPressed;
}

controllerButtonState SmartController::updateButton(ControllerButton button){
  if(button.isPressed()){
    return controllerButtonState::isPressed;
  }
  if(button.changedToPressed()){
    return controllerButtonState::changedToPressed;
  }
  return controllerButtonState::notPressed;
}

void SmartController::update(){
  leftY = controller.getAnalog(ControllerAnalog::leftY);
  leftX = controller.getAnalog(ControllerAnalog::leftX);
  rightY = controller.getAnalog(ControllerAnalog::rightY);
  rightX = controller.getAnalog(ControllerAnalog::rightY);
  updateButton(forkUp);
  updateButton(forkDown);
  updateButton(btnUp);
  updateButton(btnDown);
  updateButton(toggleMaxSpeedButton);
  updateButton(autoDistanceButton);
  updateButton(toggleDrivePolarityButton);
  updateButton(autoButton);
  updateButton(driveReverseButton);
  updateButton(toggleIntakeButton);
  updateButton(autoDistanceButton2);
  updateButton(shootButton);
}

controllerButtonState SmartController::getButtonState(controllerButtonNames button){
  switch(button){
    case 0:
      return L1;
    case 1:
      return L2;
    case 2:
      return R1;
    case 3:
      return R2;
    case 4:
      return up;
    case 5:
      return down;
    case 6:
      return left;
    case 7:
      return right;
    case 8:
      return X;
    case 9:
      return B;
    case 10:
      return Y;
    case 11:
      return A;
  }
}

float SmartController::getJoystickAxis(controllerAxisNames axis){
  switch(axis){
    case 0:
      return leftY;
    case 1:
      return leftX;
    case 2:
      return rightY;
    case 3:
      return rightX;
  }
}

bool SmartController::isButtonState(controllerButtonNames button, controllerButtonState state){
  controllerButtonState buttonState = getButtonState(button);
  if(buttonState == state){
    return true;
  } else {
    return false;
  }
}
