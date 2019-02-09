#include <iostream>
#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

#include "main.h"

// Motor Definitions
const std::uint8_t DRIVETRAIN_R_F = 3;
const std::uint8_t DRIVETRAIN_R_B = 4;
const std::uint8_t DRIVETRAIN_L_F = 1;
const std::uint8_t DRIVETRAIN_L_B  = 2;
//const std::uint8_t FORK_MOTOR = 20;
const std::uint8_t FORK_MOTOR = 17;
const std::uint8_t  INTAKE_MOTOR = 6;
const std::uint8_t  LIFT_MOTOR = 9;
const std::uint8_t  LAUNCH_MOTOR = 8;

//Vision sensor definitions
const std::uint8_t VISION_1  = 11;
const std::uint8_t VISION_2  = 12;
const std::uint8_t VISION_3  = 13;
const std::uint8_t VISION_4 = 14;

const std::uint8_t RED_SIG  = 0;
const std::uint8_t BLUE_SIG  = 1;
const std::uint8_t GREEN_SIG  = 2;
const std::uint8_t YELLOW_SIG = 3;

// Legacy port definitions - A-H (1-8)
#define ULTRA_ECHO_PORT_LEFT 1
#define ULTRA_PING_PORT_LEFT 2
#define ULTRA_ECHO_PORT 3
#define ULTRA_PING_PORT 4
#define LIFT_POTENTIOMETER_PORT 5
#define GYRO_PORT 6
#define LIMIT_PORT 8

#endif
