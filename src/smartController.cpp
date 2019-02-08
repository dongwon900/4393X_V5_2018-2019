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
  leftY = 0;
  leftX = 0;
  rightY = 0;
  rightX = 0;
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

controllerButtonState SmartController::buttonStatefromControllerButton(ControllerButton button){
  if(button.isPressed()){
    return controllerButtonState::isPressed;
  } else {
    return controllerButtonState::notPressed;
  }
}

controllerButtonState SmartController::buttonStateFromButtonIndex(int buttonIndex){
  switch(buttonIndex){
    case 0:
      return buttonStatefromControllerButton(forkUp);
    case 1:
      return buttonStatefromControllerButton(forkDown);
    case 2:
      return buttonStatefromControllerButton(btnUp);
    case 3:
      return buttonStatefromControllerButton(btnDown);
    case 4:
      return buttonStatefromControllerButton(toggleMaxSpeedButton);
    case 5:
      return buttonStatefromControllerButton(autoDistanceButton);
    case 6:
      return buttonStatefromControllerButton(recordAutoDataButton);
    case 7:
      return buttonStatefromControllerButton(autoButton);
    case 8:
      return buttonStatefromControllerButton(toggleDriveStateButton);
    case 9:
      return buttonStatefromControllerButton(toggleIntakeButton);
    case 10:
      return buttonStatefromControllerButton(miscButton);
    case 11:
      return buttonStatefromControllerButton(shootButton);
  }
}

controllerButtonState SmartController::evaluateButton(int buttonIndex){
  if(buttonStateFromButtonIndex(buttonIndex) == controllerButtonState::isPressed){
    if(!isButtonChangedToPressed[buttonIndex]){
      isButtonChangedToPressed[buttonIndex] = true;
      return controllerButtonState::changedToPressed;
    } else if(isButtonChangedToPressed[buttonIndex]){
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
  return evaluateButton(buttonIndex);
}

bool SmartController::inRange(int low, int high, int x){
  return x < high && x > low;
}

bool SmartController::vectorDataCloseEnough(std::vector<int> newData){
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
  //saving to vector
  std::vector<int> internalStorage {(int)Yleft, (int)Xleft, (int)Yright, (int)Xright, L1, L2, R1, R2, up, down, left, right, X, B, Y, A, (currentMillis - startMillis)};

  if(vectorDataCloseEnough(internalStorage)){ //checking to see if the data is "different" enough to justify saving
    return;
  }

  autoLog.push_back(internalStorage);
}

void SmartController::update(){
  leftY = controller.getAnalog(ControllerAnalog::leftY);
  leftX = controller.getAnalog(ControllerAnalog::leftX);
  rightY = controller.getAnalog(ControllerAnalog::rightY);
  rightX = controller.getAnalog(ControllerAnalog::rightY);

  for(unsigned int i = 0; i < 12; i++){
    buttonStates[i] = updateButton(i);
  }

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
  if(autoData.size() == 1){
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

  for(unsigned int i = 0; i < newData.size(); i++){
    buttonStates[i] = intToButtonState(newData[i])
  }

  autoData.erase(autoData.begin());
}

void SmartController::autonomousUpdate(std::vector<std::vector<int>>& autoData){
  currentMillis = pros::millis();
  autoLogParser(autoData);
}

static SmartController::SmartController& instance(){
  if(!inst){
    inst = new SmartController;
    return *inst;
  }
  return *inst;
}
