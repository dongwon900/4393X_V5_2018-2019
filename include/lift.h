#include "main.h"
#include "definitions.h"

extern Motor liftMotor;

class Lift{
private:
  Motor liftMotor;
  int potValue;
  int liftIndex;
	bool liftSet;
  const std::vector<int> liftPositions {1630, 2100, 2600, 2900};
public:
  Lift();
  ~Lift();
  void updatePot();
  void manualLiftControl();
  void raiseLiftIndex();
	void lowerLiftIndex();
	void updateLiftIndex();
  void updateLiftPosition();
  void update();
};
