#include "smartController.h"

SmartController::SmartController()
:Controller controller(),
ControllerButton btnUp(ControllerDigital::R1),
ControllerButton btnDown(ControllerDigital::R2),
ControllerButton forkUp(ControllerDigital::L1),
ControllerButton forkDown(ControllerDigital::L2),
ControllerButton shootButton(ControllerDigital::A),
ControllerButton autoDistanceButton(ControllerDigital::down),
ControllerButton autoButton(ControllerDigital::right),
ControllerButton driveReverseButton(ControllerDigital::X),
ControllerButton toggleMaxSpeedButton(ControllerDigital::up),
ControllerButton toggleIntakeButton(ControllerDigital::B),
ControllerButton autoDistanceButton2(ControllerDigital::Y),
ControllerButton toggleDrivePolarityButton(ControllerDigital::left) {
  leftY = 0;
  leftX = 0;
  rightY = 0;
  rightX = 0;
  L1 = ControllerButtonState::notPressed;
  L2 = ControllerButtonState::notPressed;
  R1 = ControllerButtonState::notPressed;
  R2 = ControllerButtonState::notPressed;
  up = ControllerButtonState::notPressed;
  down = ControllerButtonState::notPressed;
  left = ControllerButtonState::notPressed;
  right = ControllerButtonState::notPressed;
  X = ControllerButtonState::notPressed;
  B = ControllerButtonState::notPressed;
  Y = ControllerButtonState::notPressed;
  A = ControllerButtonState::notPressed;
}

SmartController::~SmartController(){
  leftY = 0;
  leftX = 0;
  rightY = 0;
  rightX = 0;
  L1 = ControllerButtonState::notPressed;
  L2 = ControllerButtonState::notPressed;
  R1 = ControllerButtonState::notPressed;
  R2 = ControllerButtonState::notPressed;
  up = ControllerButtonState::notPressed;
  down = ControllerButtonState::notPressed;
  left = ControllerButtonState::notPressed;
  right = ControllerButtonState::notPressed;
  X = ControllerButtonState::notPressed;
  B = ControllerButtonState::notPressed;
  Y = ControllerButtonState::notPressed;
  A = ControllerButtonState::notPressed;
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

bool SmartController::isbuttonState(controllerButtonNames button, controllerButtonState state){
  controllerButtonState buttonState = getButtonState(button);
  if(buttonState == state){
    return true;
  } else {
    return false;
  }
}
