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
const std::uint8_t FORK_MOTOR = 18;
const std::uint8_t  INTAKE_MOTOR = 6;
const std::uint8_t  LIFT_MOTOR = 13;
const std::uint8_t  LAUNCH_MOTOR = 8;

//Vision sensor definitions
const std::uint8_t AIMING_VISION_SENSOR  = 11;
const std::uint8_t POLE_VISION_SENSOR  = 12;

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
#define INTAKE_LIMIT_PORT 6
#define LIFT_LIMIT_PORT 7
#define LIMIT_PORT 8

// E_MOTOR_BRAKE_COAST = 0, // Motor coasts when stopped, traditional behavior
// E_MOTOR_BRAKE_BRAKE = 1, // Motor brakes when stopped
// E_MOTOR_BRAKE_HOLD  = 2; //Motor actively holds when stopped

#endif
