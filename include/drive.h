#include "main.h"

extern Motor driveLeftF;
extern Motor driveLeftB;
extern Motor driveRightF;
extern Motor driveRightB;

class Drive{
private:
  Motor driveLeftF;
  Motor driveLeftB;
  Motor driveRightF;
  Motor driveRightB;
  int driveState;
  int currentVoltageIndex;
  int gyroAngle;
public:
  Drive();
  ~Drive();
  void updateGyro();
  void updateSonics();
  void driveLeft(int voltage);
	void driveRight(int voltage);
	void driveAll(int leftVoltage, int rightVoltage);
  void toggleMaxSpeed();
	void toggleDriveState();
  std::vector<bool> sonicDistanceAdjust(int leftDistance, int rightDistance);
  void adjustDistance(int lefTarget, int rightTarget);
  void update(int leftVoltage, int rightVoltage);
};
