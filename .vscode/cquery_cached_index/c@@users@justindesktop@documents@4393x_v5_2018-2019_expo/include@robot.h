#include "main.h"
using namespace okapi;

//Motor definitions
Motor liftMotor(LIFT_MOTOR);
Motor forkMotor(FORK_MOTOR);
Motor intakeMotor(INTAKE_MOTOR);
Motor launcherMotor(LAUNCH_MOTOR);
Motor leftDriveF(DRIVETRAIN_L_F);
Motor leftDriveB(DRIVETRAIN_L_B);
Motor rightDriveF(DRIVETRAIN_R_F);
Motor rightDriveB(DRIVETRAIN_R_B);

// Legacy port definitions - A-H (1-8)
#define ULTRA_ECHO_PORT_LEFT 1
#define ULTRA_PING_PORT_LEFT 2
#define ULTRA_ECHO_PORT 3
#define ULTRA_PING_PORT 4
#define LIFT_POTENTIOMETER_PORT 5
#define GYRO_PORT 6
#define LIMIT_PORT 8

// Legacy Sensor Initialization
ADIButton launcherLimitSwitch(LIMIT_PORT);
ADIGyro gyro(GYRO_PORT);
pros::ADIPotentiometer liftPotentiometer (LIFT_POTENTIOMETER_PORT);
pros::ADIUltrasonic ultrasonicRight (ULTRA_ECHO_PORT, ULTRA_PING_PORT);
pros::ADIUltrasonic ultrasonicLeft (ULTRA_ECHO_PORT_LEFT, ULTRA_PING_PORT_LEFT);

// Flags
static bool loaded;
static int reversed = 1;
static int currentVoltageIndex = 0;

class Robot{
public:
  Robot();
private:
  bool loaded;
  int driveState;
  int currentVoltageIndex = 0;
};
