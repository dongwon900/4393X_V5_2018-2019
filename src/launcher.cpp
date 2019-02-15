#include "launcher.h"

Launcher::Launcher()
  :launcherMotor(LAUNCH_MOTOR),
  launcherLimitSwitch(LIMIT_PORT),
  smartController{SmartController::instance()}
{
  launcherLimit = launcherLimitSwitch.isPressed();
  launcherEnabled = false;
}

Launcher::~Launcher(){
  launcherMotor.move_voltage(0);
}

void Launcher::updateController(){
  smartController = SmartController::instance();
}

void Launcher::updateLimit(){
  launcherLimit = launcherLimitSwitch.isPressed();
}

void Launcher::enableLauncher() {
  launcherEnabled = true;
}

void Launcher::launcherAutoMovement(){
  if (smartController.isButtonState(controllerButtonNames::A, controllerButtonState::changedToPressed)) {
    launcherEnabled = true;
  }

  if (launcherEnabled) {
  	if(launcherLimit == 1){
  		if(smartController.isButtonState(controllerButtonNames::A, controllerButtonState::isPressed)){
  			launcherMotor.move_voltage(12000);
  		} else {
  			launcherMotor.move_voltage(500);
  		}
  	}	else {
  		launcherMotor.move_voltage(12000);
  	}
  }
}

void Launcher::update(){
  updateController();
  updateLimit();
	launcherAutoMovement();
}
