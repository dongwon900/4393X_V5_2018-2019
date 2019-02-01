#include "main.h"
#include "enums.h"

class Display{
private:
  Alliance alliance;
  StartingTile startingTile;
public:
  Display();
  ~Display();
  void displaySensorValuesOnBrain();
  void displayDataOnController();
  bool chooseAlliance(); //takes a button press on the lcd emulator
  bool chooseTile(); //takes a button press on the lcd emulator
  void chooseAuto();
  void update();
};
