#ifndef _DRIVETRAIN_H_
#define _DRIVETRAIN_H_

#include "main.h"
#include "definitions.h"
#include "enums.h"
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
  SmartController smartController;
public:
  Drivetrain();
  ~Drivetrain();
  void updateController();
  void updateGyro();
  void updateSonics();
  void driveLeft(int voltage);
	void driveRight(int voltage);
	void driveAll(int leftVoltage, int rightVoltage);
  void toggleMaxSpeed();
	void toggleDriveState();
  bool inRange(float low, float high, float x);
  std::vector<bool> sonicDistanceAdjust(int leftDistance, int rightDistance);
  void adjustDistance(int lefTarget, int rightTarget, float leftY, float rightY);
  void update(float leftVoltage, float rightVoltage);
};

#endif
