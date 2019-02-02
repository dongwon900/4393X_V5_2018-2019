#ifndef _DRIVETRAIN_H_
#define _DRIVETRAIN_H_

#include "main.h"
#include "definitions.h"
#include "smartController.h"

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
  void update(float leftVoltage, float rightVoltage);
};

#endif
