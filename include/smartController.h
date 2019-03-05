#ifndef _SMARTCONTROLLER_H_
#define _SMARTCONTROLLER_H_

#include "main.h"
#include "enums.h"

//built on the okapi api controller and controllerButton, has the same functionality and more
class SmartController{
  public:
    static SmartController* inst;
    okapi::Controller controller;
    okapi::ControllerButton L1;
    okapi::ControllerButton L2;
    okapi::ControllerButton R1;
    okapi::ControllerButton R2;
    okapi::ControllerButton up;
    okapi::ControllerButton down;;
    okapi::ControllerButton left;
    okapi::ControllerButton right;
    okapi::ControllerButton X;
    okapi::ControllerButton B;
    okapi::ControllerButton Y;
    okapi::ControllerButton A;
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
    static SmartController& instance();
    void operator=(const SmartController& controller);
    void initialize();
    ~SmartController();
    controllerButtonState buttonStateFromControllerButton(ControllerButton button);
    controllerButtonState buttonStateFromButtonIndex(int buttonIndex);
    controllerButtonState evaluateButton(int buttonIndex);
    controllerButtonState updateButton(controllerButtonNames button);
    bool inRange(int low, int high, int x);
    controllerButtonNames intToButtonName(int buttonIndex);
    void update();
    controllerButtonState getButtonState(controllerButtonNames button);
    float getJoystickAxis(controllerAxisNames axis);
    bool isButtonState(controllerButtonNames button, controllerButtonState state);
    controllerButtonState intToButtonState(int x);
    int controllerButtonStateToInt(controllerButtonState buttonState);

    //Methods relating to saving of data
    bool vectorDataCloseEnough(std::vector<int> newData, int resAdjust);
    void saveDataToAutoLog();
    void autoLogParser(std::vector<std::vector<int>>& autoData);
    void autonomousUpdate(std::vector<std::vector<int>>& autoData);
    void saveData(std::string data);
    std::vector<int> getSmartControllerLine();
    void getAutoLog(int autoLogIndex);
};

class CSVWriter {
  private:
    std::string fileName;
    std::string delimeter;
    int lineCount;
  public:
    CSVWriter(std::string filename);
    int getLineCount();
    //Method to append a range to the file
    template <typename T>
    void addDataInRow(T first, T last);
    //Method to append nested ranges to the file
    template <typename T>
    void addNestedRanges(T first, T last);
};

class CSVReader {
  private:
    std::string fileName;
    std::string delimeter;
  public:
      CSVReader(std::string filename);
      //Method to fetch data from a CSV File
	    std::vector<std::vector<std::string>> getData();
};

class CSVInterface {
  private:
    CSVWriter writer;
    CSVReader reader;
    std::string fileName;
    std::string delimeter;
    int lineCount;
  public:
    CSVInterface(std::string filename);
    int getLineCount();
    template <typename T>
    void addData(T first, T last);
    template <typename T>
    void addNestedData(T first, T last);
    std::vector<std::vector<std::string>> getData();
    std::vector<std::vector<int>> getDataAsInt();
};

#endif
