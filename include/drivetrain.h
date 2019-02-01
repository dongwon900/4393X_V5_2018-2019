#include "main.h"
#include "definitions.h"
#include "smartController.h"

extern Motor driveLeftF;
extern Motor driveLeftB;
extern Motor driveRightF;
extern Motor driveRightB;
extern pros::ADIGyro gyro;
extern pros::ADIUltrasonic ultrasonicLeft;
extern pros::ADIUltrasonic ultrasonicRight;

class Drivetrain{
private:
  Motor driveLeftF;
  Motor driveLeftB;
  Motor driveRightF;
  Motor driveRightB;
  pros::ADIGyro gyro;
  pros::ADIUltrasonic ultrasonicLeft;
  pros::ADIUltrasonic ultrasonicRight;
  int driveState;
  int currentVoltageIndex;
  int gyroAngle;
  int leftSonic;
  int rightSonic;
public:
  Drivetrain();
  ~Drivetrain();
  void updateGyro();
  void updateSonics();
  void driveLeft(int voltage);
	void driveRight(int voltage);
	void driveAll(int leftVoltage, int rightVoltage);
  void toggleMaxSpeed();
	void toggleDriveState();
  bool inRange(float low, float high, float x);
  std::vector<bool> sonicDistanceAdjust(int leftDistance, int rightDistance);
  void adjustDistance(int lefTarget, int rightTarget);
  void update(int leftVoltage, int rightVoltage);
};
