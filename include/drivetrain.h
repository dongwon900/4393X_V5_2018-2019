#ifndef _DRIVETRAIN_H_
#define _DRIVETRAIN_H_

#include "main.h"
#include "definitions.h"
#include "smartController.h"

class Drivetrain{
public:
  Motor driveLeftF;
  Motor driveLeftB;
  Motor driveRightF;
  Motor driveRightB;
  pros::ADIUltrasonic ultrasonicLeft;
  pros::ADIUltrasonic ultrasonicRight;
  int driveState;
  int currentVoltageIndex;
  int gyroAngle;
  int leftSonic;
  int rightSonic;
  double leftTarget;
  double rightTarget;
  //Motor target positions (encoder values)
  const int ticksRed = 1800;
  const int ticksGreen = 900;
  const int ticksBlue = 300;
  double wheelDiameter = 4.25;
public:
  void initialize();
  Drivetrain();
  ~Drivetrain();
  void updateSonics();
  void driveLeft(int voltage);
	void driveRight(int voltage);
	void driveAll(int leftVoltage, int rightVoltage);
  void toggleMaxSpeed();
	void toggleDriveState();
  bool inRange(float low, float high, float x);
  std::vector<bool> sonicDistanceAdjust(int leftDistance, int rightDistance);
  void adjustDistance(int lefTarget, int rightTarget);
  void autoaim();
  void update(float leftVoltage, float rightVoltage);
  void turnDegrees(double degrees);
  int velocityBasedOnDistanceLeft(int ticksRemaining);
  void driveLeftRelative(int tickCount, int velocity);
  void driveRightRelative(int tickCount, int velocity);
  void driveDistance(double inches);
};

#endif
