#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

// Motor Definitions
const std::uint8_t DRIVETRAIN_R_F = 3;
const std::uint8_t DRIVETRAIN_R_B = 4;
const std::uint8_t DRIVETRAIN_L_F = 1;
const std::uint8_t DRIVETRAIN_L_B  = 2;
//const std::uint8_t FORK_MOTOR = 20;
const std::uint8_t FORK_MOTOR = 19;
const std::uint8_t  INTAKE_MOTOR = 6;
const std::uint8_t  LIFT_MOTOR = 7;
const std::uint8_t  LAUNCH_MOTOR = 8;

//Vision sensor definitions
const std::uint8_t VISION_1  = 11;
const std::uint8_t VISION_2  = 12;
const std::uint8_t VISION_3  = 13;
const std::uint8_t VISION_4 = 14;

// Legacy port definitions - A-H (1-8)
const std::uint8_t ULTRA_ECHO_PORT_LEFT = 1;
const std::uint8_t ULTRA_PING_PORT_LEFT = 2;
const std::uint8_t ULTRA_ECHO_PORT = 3;
const std::uint8_t ULTRA_PING_PORT = 4;
const std::uint8_t LIFT_POTENTIOMETER_PORT = 5;
const std::uint8_t GYRO_PORT = 6;
const std::uint8_t LIMIT_PORT = 8;

// Controller Button Mappings
// L1 forkUp
// L2 forkDown
// R1 btnUp
// R2 btnDown
// up toggleMaxSpeedButton
// down autoDistanceButton
// left toggleDrivePolarity
// right autoButton
// X toggleDriveStateButton
// B toggleIntakeButton
// Y autoDistanceButton2
// A shootButton

#endif
