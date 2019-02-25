#include "smartController.h"

SmartController* SmartController::inst{nullptr};

//set the smartController to initial values
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

//returns a buttonState which is either isPressed or notPressed (changedToPressed is evaluated in evaluateButton)
controllerButtonState SmartController::buttonStateFromControllerButton(ControllerButton button){
  if(button.isPressed()){
    return controllerButtonState::isPressed;
  } else {
    return controllerButtonState::notPressed;
  }
}

//takes a buttonIndex which is than turned into a button name from an inOrder list
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

//takes a buttonIndex (usually gotten from updateButton) and then runs buttonStateFromButtonIndex to get the button state and returns a button state according to the previous states of the buttons
controllerButtonState SmartController::evaluateButton(int buttonIndex){
  if(buttonStateFromButtonIndex(buttonIndex) == controllerButtonState::isPressed){ //if the button is pressed continue on to check to see if it should be changedToPressed
    if(isButtonChangedToPressed[buttonIndex] == false && isButtonPressed[buttonIndex] == false){ //if the button was notPressed before than clearly it is changedToPressed
      isButtonChangedToPressed[buttonIndex] = true;
      isButtonPressed[buttonIndex] = true;
      return controllerButtonState::changedToPressed;
    } else if(isButtonChangedToPressed[buttonIndex] == false && isButtonPressed[buttonIndex] == true){
      return controllerButtonState::isPressed;
    } else {
      isButtonChangedToPressed[buttonIndex] = false;
      isButtonPressed[buttonIndex] = true;
      return controllerButtonState::isPressed;
    }
  } else { //if the button is notPressed than set everything to false and return notPressed
    isButtonChangedToPressed[buttonIndex] = false;
    isButtonPressed[buttonIndex] = false;
    return controllerButtonState::notPressed;
  }
}

//takes the button and runs evaluateButton and returns the result
controllerButtonState SmartController::updateButton(controllerButtonNames button){
  int buttonIndex = (int) button;



  return evaluateButton(buttonIndex);
}

//if the x is between the low and the high than it returns true
bool SmartController::inRange(int low, int high, int x){
  return x < high && x > low;
}

//return true to ignore the data and false to save it. The data is similar if the joysticks are within the resAdjust of the old readings and the buttons are the same
bool SmartController::vectorDataCloseEnough(std::vector<int> newData, int resAdjust){
  if(autoLog.size() < 1){ //checks to see if there is data in autoLog to compare
    return false;
  }
  std::vector<int> oldData = autoLog[autoLog.size()-1]; //loads the most recently saved data

  std::vector<int> newDataNoTStamp = newData; //doing pop backs here to get rid of the timestamp cause we dont care about it anymore
  newDataNoTStamp.pop_back();
  std::vector<int> oldDataNoTStamp = oldData;
  oldDataNoTStamp.pop_back();

  if(oldDataNoTStamp == newDataNoTStamp){ //data is the same so ignore it
    return true;
  }

  bool joystickSimilar;
  bool isButtonStatesEqual;

  for(unsigned int i = 0; i < 4; i++){ //going through and comparing the joystick values and if they are within 20 mV of each other
    if(!inRange(oldData[i]-resAdjust, oldData[i]+resAdjust, newData[i])){
      joystickSimilar = false;
    }
  }

  for(unsigned int i = 4; i < oldData.size()-2; i++){
    if(oldData[i] != newData[i]){
      isButtonStatesEqual = false;
    }
  }

  if(joystickSimilar == false || isButtonStatesEqual == false){ //if the joystick values are not close enough or there was a change in the button states then this returns false to tell the controller to save the data
    return false;
  }

  return true; //if the data passed all the other tests than the joystick value are close enough and the buttons are the same so it should not get saved
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

//updates the values of the controller using
void SmartController::update(){
  leftY = getJoystickAxis(controllerAxisNames::leftY); //updating the joystick values
  leftX =  getJoystickAxis(controllerAxisNames::leftX);
  rightY =  getJoystickAxis(controllerAxisNames::rightY);
  rightX =  getJoystickAxis(controllerAxisNames::rightX);

  for(unsigned int i = 0; i < 12; i++){ //sets the buttonState to the value from updateButton
    buttonStates[i] = updateButton(intToButtonName(i));
  }

  currentMillis = pros::millis(); //generates the timestamp

  if(buttonStates[7] == controllerButtonState::changedToPressed){ //checks the recordAutoDataButton to see if it was changedToPressed
    if(isRecording){
      isRecording = false;
    } else {
      isRecording = true;
    }
  }

  if(isRecording){ //if the recordAutoDataButton was changedToPressed then save the data
    saveDataToAutoLog();
  }
}

//gets the buttonState from the buttonStates vector
controllerButtonState SmartController::getButtonState(controllerButtonNames button){
  return buttonStates[(int)button];
}

//gets the float from a given axis of a joystick
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

//checks to see if the buttonState of a button is the same as a given state
bool SmartController::isButtonState(controllerButtonNames button, controllerButtonState state){
  return getButtonState(button) == state;
}

//takes an int and returns a corresponding buttonState from the controllerButtonState enum
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
  float newLeftY = (float)newData[0] / (float)12000;
  float newLeftX = (float)newData[1] / (float)12000;
  float newRightY = (float)newData[2] / (float)12000;
  float newRightX = (float)newData[3] / (float)12000;
  leftY = newLeftY;
  leftX = newLeftX;
  rightY = newRightY;
  rightX = newRightX;

  //takes the integers from the data and turns them to a buttonState and saves them
  for(unsigned int i = 0; i < newData.size(); i++){
    buttonStates[i] = intToButtonState(newData[i]);
  }

  autoData.erase(autoData.begin()); //erases the first section of data so it wont be copied again
}

//This method is meant to be run in a while loop so that why the timestamp is constantly updating and the data is being constantly checked
void SmartController::autonomousUpdate(std::vector<std::vector<int>>& autoData){
  currentMillis = pros::millis();
  autoLogParser(autoData);
}

//Takes a controllerButtonState and returns an integer
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

//gets a reference to the smartcontroller object
SmartController& SmartController::instance(){
  if(!inst){
    inst = new SmartController();
  }
  return *inst;
}

//sets the fields of one smartController equal to another one
//This is now defunct because smartController is singleton but I left it in since it may be useful for updating the controller is other ways
void SmartController::operator=(const SmartController& controller){
  this->leftY = controller.leftY;
  this->leftX = controller.leftX;
  this->rightY = controller.rightY;
  this->rightX = controller.rightX;
  this->buttonStates = controller.buttonStates;
  this->startMillis = controller.startMillis;
  this->currentMillis = controller.currentMillis;
  this->parsedData = controller.parsedData;
  this->timestampDiff = controller.timestampDiff;
  this->isRecording = controller.isRecording;
  this->isButtonChangedToPressed = controller.isButtonChangedToPressed;
  this->isButtonPressed = controller.isButtonPressed;
}
