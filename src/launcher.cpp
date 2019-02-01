#include "launcher.h"

Launcher::Launcher(){
  :launcherMotor{LAUNCH_MOTOR};
  ADIButton launcherLimitSwitch(LIMIT_PORT);
  launcherLimit = launcherLimitSwitch.isPressed();
}

Launcher::~Launcher(){
  launcherMotor.move_voltage(0);
}

void Launcher::launcherAutoMovement(){
	if(launcherLimit == 1){
		if(shootButton.isPressed()){
			launcherMotor.move_voltage(12000);
		} else {
			launcherMotor.move_voltage(500);
		}
	}	else {
		launcherMotor.move_voltage(12000);
	}
}

void Launcher::update(){
  launcherLimit = launcherLimitSwitch.isPressed();
	launcherAutoMovement();
}
