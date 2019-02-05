#include "smartController.h"

SmartController::SmartController()
:controller(),
forkUp(ControllerDigital::L1, false),
forkDown(ControllerDigital::L2, false),
btnUp(ControllerDigital::R1, false),
btnDown(ControllerDigital::R2, false),
toggleMaxSpeedButton(ControllerDigital::up, false),
autoDistanceButton(ControllerDigital::down, false),
recordAutoDataButton(ControllerDigital::left, false),
autoButton(ControllerDigital::right, false),
toggleDriveStateButton(ControllerDigital::X, false),
toggleIntakeButton(ControllerDigital::B, false),
autoDistanceButton2(ControllerDigital::Y, false),
shootButton(ControllerDigital::A, false)
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
  startMillis = pros::millis();
  currentMillis = startMillis;
  parsedData = false;
  timestampDiff = 0;
  isRecording = false;
  isButtonChangedToPressed.resize(12);
  isButtonPressed.resize(12);
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

controllerButtonState SmartController::evaluateButton(int buttonIndex){


  // if(button.isPressed()){
  //   return controllerButtonState::isPressed;
  // }
  // if(button.changedToPressed()){
  //   return controllerButtonState::changedToPressed;
  // }
  // // if(button.isPressed()){
  // //   return controllerButtonState::isPressed;
  // // }
  // return controllerButtonState::notPressed;
}

controllerButtonState SmartController::updateButton(controllerButtonNames button){
  switch(button){
    case 0:
      return evaluateButton(button);
    case 1:
      return evaluateButton(button);
    case 2:
      return evaluateButton(button);
    case 3:
      return evaluateButton(button);
    case 4:
      return evaluateButton(button);
    case 5:
      return evaluateButton(button);
    case 6:
      return evaluateButton(button);
    case 7:
      return evaluateButton(button);
    case 8:
      return evaluateButton(button);
    case 9:
      return evaluateButton(button);
    case 10:
      return evaluateButton(button);
    case 11:
      return evaluateButton(button);
  }
}

bool SmartController::inRange(int low, int high, int x){
  return x < high && x > low;
}

bool SmartController::vectorDataCloseEnough(std::vector<int> newData){
  if(autoLog.size() < 1){
    return false;
  }
  std::vector<int> oldData = autoLog[autoLog.size()-1];

  std::vector<int> newDataNoTStamp = newData;
  newDataNoTStamp.pop_back();
  std::vector<int> oldDataNoTStamp = oldData;
  oldDataNoTStamp.pop_back();
  if(oldDataNoTStamp == newDataNoTStamp){
    return true;
  }

  for(unsigned int i = 0; i < 4; i++){
    if(!inRange(oldData[i]-20, oldData[i]+20, newData[i])){
      return false;
    }
  }
  for(unsigned int i = 4; i < oldData.size()-2; i++){
    if(oldData[i] != newData[i]){
      return false;
    }
  }

  return true;
}

void SmartController::saveDataToAutoLog(){
  //transferring the floats to integers multiplied by the maximum voltage (so they can be pulled out and converted later)
  float Yleft = leftY * 12000;
  float Xleft = leftX * 12000;
  float Yright = rightY * 12000;
  float Xright = rightX * 12000;
  std::vector<int> internalStorage {(int)Yleft, (int)Xleft, (int)Yright, (int)Xright, L1, L2, R1, R2, up, down, left, right, X, B, Y, A, (currentMillis - startMillis)};

  if(vectorDataCloseEnough(internalStorage)){
    return;
  }

  autoLog.push_back(internalStorage);
}

void SmartController::update(){
  leftY = controller.getAnalog(ControllerAnalog::leftY);
  leftX = controller.getAnalog(ControllerAnalog::leftX);
  rightY = controller.getAnalog(ControllerAnalog::rightY);
  rightX = controller.getAnalog(ControllerAnalog::rightY);
  L1 = updateButton(forkUp, controllerButtonNames::L1);
  L2 = updateButton(forkDown, controllerButtonNames::L2);
  R1 = updateButton(btnUp, controllerButtonNames::R1);
  R2 = updateButton(btnDown, controllerButtonNames::R2);
  up = updateButton(toggleMaxSpeedButton, controllerButtonNames::up);
  down = updateButton(autoDistanceButton, controllerButtonNames::down);
  left = updateButton(toggleDrivePolarityButton, controllerButtonNames::left);
  right = updateButton(autoButton, controllerButtonNames::right);
  X = updateButton(driveReverseButton, controllerButtonNames::X);
  B = updateButton(toggleIntakeButton, controllerButtonNames::B);
  Y = updateButton(autoDistanceButton2, controllerButtonNames::Y);
  A = updateButton(shootButton, controllerButtonNames::A);
  currentMillis = pros::millis();

  if(left == controllerButtonState::changedToPressed){
    if(isRecording){
      isRecording = false;
    } else {
      isRecording = true;
    }
  }

  if(isRecording){
    saveDataToAutoLog();
  }
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

controllerButtonState intToButtonState(int x){
  if(x == 0){
    return controllerButtonState::isPressed;
  }
  if(x == 1){
    return controllerButtonState::notPressed;
  }
  if(x == 2){
    return controllerButtonState::changedToPressed;
  }
}

void SmartController::autoLogParser(std::vector<std::vector<int>>& autoData){
  if(autoData.size()-1 < 1){
    return;
  }

  std::vector<int> newData = autoData[0];

  if(parsedData){
    if(currentMillis + timestampDiff < newData[16]){
      pros::delay(newData[16] - currentMillis);
    }
  } else {
    timestampDiff = newData[16] - currentMillis;
    currentMillis = newData[16];
  }

  //converts the ints back to floats for the joysticks
  float newLeftY = (float)newData[0] / 12000;
  float newLeftX = (float)newData[1] / 12000;
  float newRightY = (float)newData[2] / 12000;
  float newRightX = (float)newData[3] / 12000;
  leftY = newLeftY;
  leftX = newLeftX;
  rightY = newRightY;
  rightX = newRightX;

  L1 = intToButtonState(newData[4]);
  L2 = intToButtonState(newData[5]);
  R1 = intToButtonState(newData[6]);
  R2 = intToButtonState(newData[7]);
  up = intToButtonState(newData[8]);
  down = intToButtonState(newData[9]);
  left = intToButtonState(newData[10]);
  right = intToButtonState(newData[11]);
  X = intToButtonState(newData[12]);
  B = intToButtonState(newData[13]);
  Y = intToButtonState(newData[14]);
  A = intToButtonState(newData[15]);

  autoData.erase(autoData.begin());
}

void SmartController::autonomousUpdate(std::vector<std::vector<int>>& autoData){
  currentMillis = pros::millis();
  autoLogParser(autoData);
}
