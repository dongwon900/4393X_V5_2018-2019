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
  //fields relating to autonomous
  bool performingAutoFunction;
  bool performingMovement;
  int currentMillis;
  int targetMillis;
public:
  //constructor, destructor
  void initialize();
  Drivetrain();
  ~Drivetrain();
  //methods relating to opcontrol
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
  //methods relating to autonomous
  int velocityBasedOnDistanceLeft(int ticksRemaining);
  void driveLeftVelocity(int velocity);
  void driveRightVelocity(int velocity);
  void driveAllVelocity(int leftVelocity, int rightVelocity);
  void driveLeftRelative(int tickCount, int velocity);
  void driveRightRelative(int tickCount, int velocity);
  void driveAllRelative(int leftTickCount, int rightTickCount, int leftVelocity, int rightVelocity);
  void setBrakeMode(pros::motor_brake_mode_e_t mode);
  void turnDegrees(double degrees, double delayRatio);
  void driveDistance(double inches, double delayRatio);
  void driveDistance(double inches);
  void turn45(int sign);
  void turn90(int sign);
  void turn180(int sign);
  void asyncMovementSequencer();
  void updateAuto();
};

#endif
