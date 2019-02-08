#include "smartController.h"

using namespace okapi;
//Controller
Controller controller;

// Controller Buttons
okapi::ControllerButton btnUp(ControllerDigital::R1);
okapi::ControllerButton btnDown(ControllerDigital::R2);
okapi::ControllerButton forkUp(ControllerDigital::L1);
okapi::ControllerButton forkDown(ControllerDigital::L2);
okapi::ControllerButton shootButton(ControllerDigital::A);
okapi::ControllerButton autoDistanceButton(ControllerDigital::down);
okapi::ControllerButton autoButton(ControllerDigital::right);
okapi::ControllerButton driveReverseButton(ControllerDigital::X);
okapi::ControllerButton toggleMaxSpeedButton(ControllerDigital::up);
okapi::ControllerButton toggleIntakeButton(ControllerDigital::B);
okapi::ControllerButton autoDistanceButton2(ControllerDigital::Y);
