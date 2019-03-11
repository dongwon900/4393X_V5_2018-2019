#include "smartController.h"

//the master POINTER
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
  recordingButton = controllerButtonNames::none;
}

void SmartController::initialize(controllerButtonNames recordingButton){
  initialize();
  this->recordingButton = recordingButton;
}

//constructor boy
SmartController::SmartController()
:controller(),
L1(ControllerDigital::L1, false),
L2(ControllerDigital::L2, false),
R1(ControllerDigital::R1, false),
R2(ControllerDigital::R2, false),
up(ControllerDigital::up, false),
down(ControllerDigital::down, false),
left(ControllerDigital::left, false),
right(ControllerDigital::right, false),
X(ControllerDigital::X, false),
B(ControllerDigital::B, false),
Y(ControllerDigital::Y, false),
A(ControllerDigital::A, false) {
  initialize();
}

SmartController::SmartController(controllerButtonNames recordingButton):SmartController()
{
  this->recordingButton = recordingButton;
}

//destructor boy
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
    case 1:
      return buttonStateFromControllerButton(this->L1);
    case 2:
      return buttonStateFromControllerButton(this->L2);
    case 3:
      return buttonStateFromControllerButton(this->R1);
    case 4:
      return buttonStateFromControllerButton(this->R2);
    case 5:
      return buttonStateFromControllerButton(this->up);
    case 6:
      return buttonStateFromControllerButton(this->down);
    case 7:
      return buttonStateFromControllerButton(this->left);
    case 8:
      return buttonStateFromControllerButton(this->right);
    case 9:
      return buttonStateFromControllerButton(this->X);
    case 10:
      return buttonStateFromControllerButton(this->B);
    case 11:
      return buttonStateFromControllerButton(this->Y);
    case 12:
      return buttonStateFromControllerButton(this->A);
  }
}

//takes a buttonIndex (usually gotten from updateButton) and then runs buttonStateFromButtonIndex to get the button state and returns a button state according to the previous states of the buttons
controllerButtonState SmartController::evaluateButton(int buttonIndex){
  if(buttonStateFromButtonIndex(buttonIndex) == controllerButtonState::isPressed){ //if the button is pressed continue on to check to see if it should be changedToPressed
    if(isButtonChangedToPressed[buttonIndex] == false && isButtonPressed[buttonIndex] == false){ //if the button was notPressed before than clearly it is changedToPressed
      isButtonChangedToPressed[buttonIndex] = true;
      isButtonPressed[buttonIndex] = false;
      return controllerButtonState::changedToPressed;
    } else if(isButtonChangedToPressed[buttonIndex] == true && isButtonPressed[buttonIndex] == false){
      isButtonChangedToPressed[buttonIndex] = false;
      isButtonPressed[buttonIndex] = true;
      return controllerButtonState::isPressed;
    } else if(isButtonChangedToPressed[buttonIndex] == false && isButtonPressed[buttonIndex] == true){
      return controllerButtonState::isPressed;
    } else if(isButtonChangedToPressed[buttonIndex] == true && isButtonPressed[buttonIndex] == true){
      isButtonChangedToPressed[buttonIndex] == false;
      return controllerButtonState::isPressed;
    }
  } else if(isButtonPressed[buttonIndex] == true){ //if the button is notPressed than set everything to false and return notPressed
    isButtonPressed[buttonIndex] = false;
    return controllerButtonState::changedToNotPressed;
  } else if(isButtonPressed[buttonIndex] == false){
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

//saves the current smartController state to the autoLog field
void SmartController::saveDataToAutoLog(){
  //transferring the floats to integers multiplied by the maximum voltage (so they can be pulled out and converted later)
  float Yleft = leftY * 12000;
  float Xleft = leftX * 12000;
  float Yright = rightY * 12000;
  float Xright = rightX * 12000;
  //saving to vector
  std::vector<int> internalStorage;
  internalStorage.push_back((int)leftY);
  internalStorage.push_back((int)leftX);
  internalStorage.push_back((int)rightY);
  internalStorage.push_back((int)rightX);
  for(unsigned int i = 0; i < buttonStates.size(); i++){
    internalStorage.push_back(controllerButtonStateToInt(buttonStates[i]));
  }

  if(vectorDataCloseEnough(internalStorage, 20)){ //checking to see if the data is "different" enough to justify saving
    return;
  }

  autoLog.push_back(internalStorage);
}

//takes an integer and turns it into a controllerButtonNames
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

  //saves the collected data for a cycle to the autoLog
  if(isRecording){
    saveDataToAutoLog();
    printf("I am recording");
  } else {
    printf("Not recording");
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

//Takes a the data from the SD card
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
  return (int)buttonState;
}

//gets a reference to the smartcontroller object with no recordingbutton
//once you generate the smartcontroller with or without a recordingbutton you cannot generate one with the other orientation
SmartController& SmartController::instance(){
  if(!inst){
    inst = new SmartController();
  }
  return *inst;
}

//gets a reference to the smartController object with a recordingButton
//once you generate the smartcontroller with or without a recordingbutton you cannot generate one with the other orientation
SmartController& SmartController::instance(controllerButtonNames recordingButton){
  if(!inst){
    inst = new SmartController(recordingButton);
  }
  return *inst;
}

//sets the fields of one smartController equal to another one
//This is now defunct because smartController is singleton but I left it in since it may be useful for updating the controller is other ways
void SmartController::operator=(const SmartController& controller){
  this->inst = controller.inst;
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
  this->autoLog = controller.autoLog;
}

//takes a filename and saves the data from the controller to the SD card in a CSV
void SmartController::saveDataToSDCard(std::string filename){
  CSVInterface interface(filename);
  interface.addNestedData(autoLog.begin(), autoLog.end());
}

//takes a filename and gets a file with that name and loads the data to the autoLog
void SmartController::loadDataFromSDCard(std::string filename){
  CSVInterface interface(filename);
  autoLog = interface.getDataAsInt();
}

//takes a string and removes the delimeter and returns each item has an index in a vector
std::vector<std::string> split(const std::string& str, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(str);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}

//CSVWriter is used to write data as a CSV to the SDCARD
CSVWriter::CSVWriter(std::string filename)
:fileName(filename), delimeter(","), lineCount(0)
{
}

//return the lineCount
int CSVWriter::getLineCount(){
  return lineCount;
}

template <typename T>
//This method takes a range of data and appends it as a row with a delimeter (default is comma)
void CSVWriter::addDataInRow(T first, T last){
  std::fstream file;
	// Open the file in truncate mode if first line else in Append Mode
	file.open(fileName, std::ios::out | (lineCount ? std::ios::app : std::ios::trunc));

  // Iterate over the range and add each lement to file seperated by delimeter.
	for (; first != last; ) {
		file << *first;
		if (++first != last)
			file << delimeter;
	}
	file << "\n";
	lineCount++;

	// Close the file
	file.close();
}

template <typename T>
//This method appends a nested set of ranges with a delimeter (default is comma)
void CSVWriter::addNestedRanges(T first, T last){
  std::fstream file;
	// Open the file in truncate mode if first line else in Append Mode
	file.open(fileName, std::ios::out | (lineCount ? std::ios::app : std::ios::trunc));

  // Iterate over the range and add each lement to file seperated by delimeter.
	for (; first != last; ) {
		addDataInRow((*first).begin(), (*first).end());
	}

	// Close the file
	file.close();
}

//CSVReader loads data as a CSV from the SDCARD
CSVReader::CSVReader(std::string filename)
:fileName(filename), delimeter(",")
{
}

//Gets the data from the SDCard and returns it
std::vector<std::vector<std::string>> CSVReader::getData(){
  std::ifstream file(fileName);

	std::vector<std::vector<std::string> > dataList;

	std::string line = "";
  char delim = ',';
	// Iterate through each line and split the content using delimeter
	while (getline(file, line))
	{
		std::vector<std::string> vec;
		vec = split(line, delim);
		dataList.push_back(vec);
	}
	// Close the File
	file.close();

	return dataList;
}

//CSVInterface is the wrapper for the CSVReader and the CSVWriter to allow easy use of CSV files on the SDCARD
CSVInterface::CSVInterface(std::string filename)
:writer(filename), reader(filename), fileName(filename), delimeter(","), lineCount(this->writer.getLineCount())
{
}

//Gets the lineCount of the file, if known
int CSVInterface::getLineCount(){
  lineCount = writer.getLineCount();
  return writer.getLineCount();
}

//Adds data to the file from the first and last iterator
template <typename T>
void CSVInterface::addData(T first, T last){
  writer.addDataInRow(first, last);
}

//Adds nestedData using the first and last iterator of the parent object
template <typename T>
void CSVInterface::addNestedData(T first, T last){
  writer.addNestedRanges(first, last);
}

//Gets the data from the SDCard
std::vector<std::vector<std::string>> CSVInterface::getData(){
  return reader.getData();
}

//Gets the data and converts it to intergers for ease of use
std::vector<std::vector<int>> CSVInterface::getDataAsInt(){
  std::vector<std::vector<std::string>> data = reader.getData();
  std::vector<std::vector<int>> dataAsInt;

  for(unsigned int i = 0; i < data.size(); i++){
    std::vector<int> dataInLine;
    for(unsigned int n = 0; n < data[i].size(); n++){
      dataInLine.push_back(std::stoi(data[i][n]));
    }
    dataAsInt.push_back(dataInLine);
  }

  return dataAsInt;
}
