#include "smartController.h"

SmartController* SmartController::inst{nullptr};

void SmartController::initialize(){
  leftY = 0;
  leftX = 0;
  rightY = 0;
  rightX = 0;
  buttonStates.resize(12);
  startMillis = pros::millis();
  currentMillis = startMillis;
  parsedData = false;
  timestampDiff = 0;
  isRecording = false;
  isButtonChangedToPressed.resize(12);
  isButtonPressed.resize(12);
  for(unsigned int i = 0; i < 12; i++){
    buttonStates[i] = controllerButtonState::notPressed;
  }
  for(unsigned int i = 0; i < 12; i++){
    isButtonChangedToPressed[i] = false;
  }
  for(unsigned int i = 0; i < 12; i++){
    isButtonPressed[i] = false;
  }
}

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
miscButton(ControllerDigital::Y, false),
shootButton(ControllerDigital::A, false) {
  initialize();
}

SmartController::~SmartController(){

}

controllerButtonState SmartController::buttonStateFromControllerButton(ControllerButton button){
  if(button.isPressed()){
    return controllerButtonState::isPressed;
  } else {
    return controllerButtonState::notPressed;
  }
}

controllerButtonState SmartController::buttonStateFromButtonIndex(int buttonIndex){
  switch(buttonIndex){
    case 0:
      return buttonStateFromControllerButton(forkUp);
    case 1:
      return buttonStateFromControllerButton(forkDown);
    case 2:
      return buttonStateFromControllerButton(btnUp);
    case 3:
      return buttonStateFromControllerButton(btnDown);
    case 4:
      return buttonStateFromControllerButton(toggleMaxSpeedButton);
    case 5:
      return buttonStateFromControllerButton(autoDistanceButton);
    case 6:
      return buttonStateFromControllerButton(recordAutoDataButton);
    case 7:
      return buttonStateFromControllerButton(autoButton);
    case 8:
      return buttonStateFromControllerButton(toggleDriveStateButton);
    case 9:
      return buttonStateFromControllerButton(toggleIntakeButton);
    case 10:
      return buttonStateFromControllerButton(miscButton);
    case 11:
      return buttonStateFromControllerButton(shootButton);
  }
}

controllerButtonState SmartController::evaluateButton(int buttonIndex){
  if(buttonStateFromButtonIndex(buttonIndex) == controllerButtonState::isPressed){
    if(isButtonChangedToPressed[buttonIndex] == false){
      isButtonChangedToPressed[buttonIndex] = true;
      return controllerButtonState::changedToPressed;
    } else {
      isButtonChangedToPressed[buttonIndex] = false;
      isButtonPressed[buttonIndex] = true;
      return controllerButtonState::isPressed;
    }
  } else {
    isButtonChangedToPressed[buttonIndex] = false;
    isButtonPressed[buttonIndex] = false;
    return controllerButtonState::notPressed;
  }
}

controllerButtonState SmartController::updateButton(controllerButtonNames button){
  int buttonIndex = (int) button;

  if(evaluateButton(buttonIndex) == controllerButtonState::changedToPressed){
    printf("%s\n", "changed to pressed dawg");
  }

  return evaluateButton(buttonIndex);
}

bool SmartController::inRange(int low, int high, int x){
  return x < high && x > low;
}

bool SmartController::vectorDataCloseEnough(std::vector<int> newData, int resAdjust){
  if(autoLog.size() < 1){
    return false;
  }
  std::vector<int> oldData = autoLog[autoLog.size()-1];

  std::vector<int> newDataNoTStamp = newData; //doing pop backs here to get rid of the timestamp cause we dont care about it anymore
  newDataNoTStamp.pop_back();
  std::vector<int> oldDataNoTStamp = oldData;
  oldDataNoTStamp.pop_back();
  if(oldDataNoTStamp == newDataNoTStamp){
    return true;
  }

  for(unsigned int i = 0; i < 4; i++){
    if(!inRange(oldData[i]-resAdjust, oldData[i]+resAdjust, newData[i])){
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
  //saving to vector
  std::vector<int> internalStorage {(int)Yleft, (int)Xleft, (int)Yright, (int)Xright, L1, L2, R1, R2, up, down, left, right, X, B, Y, A, (currentMillis - startMillis)};

  if(vectorDataCloseEnough(internalStorage, 20)){ //checking to see if the data is "different" enough to justify saving
    return;
  }

  autoLog.push_back(internalStorage);
}

controllerButtonNames SmartController::intToButtonName(int buttonIndex){
  switch(buttonIndex){
    case 0:
      return controllerButtonNames::L1;
    case 1:
      return controllerButtonNames::L2;
    case 2:
      return controllerButtonNames::R1;
    case 3:
      return controllerButtonNames::R2;
    case 4:
      return controllerButtonNames::up;
    case 5:
      return controllerButtonNames::down;
    case 6:
      return controllerButtonNames::left;
    case 7:
      return controllerButtonNames::right;
    case 8:
      return controllerButtonNames::X;
    case 9:
      return controllerButtonNames::B;
    case 10:
      return controllerButtonNames::Y;
    case 11:
      return controllerButtonNames::A;
  }
}

void SmartController::update(){
  leftY = controller.getAnalog(ControllerAnalog::leftY);
  leftX = controller.getAnalog(ControllerAnalog::leftX);
  rightY = controller.getAnalog(ControllerAnalog::rightY);
  rightX = controller.getAnalog(ControllerAnalog::rightY);

  for(unsigned int i = 0; i < 12; i++){
    buttonStates[i] = updateButton(intToButtonName(i));
  }

  currentMillis = pros::millis();

  if(buttonStates[7] == controllerButtonState::changedToPressed){
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
  return buttonStates[(int)button];
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
  return getButtonState(button) == state;
}

controllerButtonState intToButtonState(int x){
  switch(x){
    case 0:
      return controllerButtonState::isPressed;
    case 1:
      return controllerButtonState::notPressed;
    case 2:
      return controllerButtonState::changedToPressed;
    default:
      return controllerButtonState::notPressed;
  }
}

void SmartController::autoLogParser(std::vector<std::vector<int>>& autoData){
  if(autoData.size() == 1){
    return;
  }

  std::vector<int> newData = autoData[0];

  if(parsedData){
    if(currentMillis + timestampDiff < newData[16]){ //move to while loop
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

  for(unsigned int i = 0; i < newData.size(); i++){
    buttonStates[i] = intToButtonState(newData[i]);
  }

  autoData.erase(autoData.begin());
}

void SmartController::autonomousUpdate(std::vector<std::vector<int>>& autoData){
  currentMillis = pros::millis();
  autoLogParser(autoData);
}

int SmartController::controllerButtonStateToInt(controllerButtonState buttonState){
  if(buttonState == controllerButtonState::isPressed){
    return 0;
  } else if(buttonState == controllerButtonState::notPressed){
    return 1;
  } else if(buttonState == controllerButtonState::changedToPressed){
    return 2;
  } else {
    return 1;
  }
}

std::string SmartController::printButtonStates(){

}

SmartController& SmartController::instance(){
  if(!inst){
    inst = new SmartController();
  }
  return *inst;
}
