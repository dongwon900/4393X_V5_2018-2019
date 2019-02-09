#include "drivetrain.h"

void Drivetrain::initialize(){
  gyroAngle = gyro.get_value();
  leftSonic = ultrasonicLeft.get_value();
  rightSonic = ultrasonicRight.get_value();
  driveState = 1;
  currentVoltageIndex = 10000;
  leftTarget = 0;
  rightTarget = 0;
  driveLeftF.move_voltage(0);
  driveLeftB.move_voltage(0);
  driveRightF.move_voltage(0);
  driveRightB.move_voltage(0);
}

Drivetrain::Drivetrain()
:driveLeftF(DRIVETRAIN_L_F),
driveLeftB(DRIVETRAIN_L_B),
driveRightF(-DRIVETRAIN_R_F),
driveRightB(-DRIVETRAIN_R_B),
gyro(GYRO_PORT),
ultrasonicLeft (ULTRA_ECHO_PORT_LEFT, ULTRA_PING_PORT_LEFT),
ultrasonicRight (ULTRA_ECHO_PORT, ULTRA_PING_PORT) {
  initialize();
}

Drivetrain::~Drivetrain(){
  driveLeftF.move_voltage(0);
  driveLeftB.move_voltage(0);
  driveRightF.move_voltage(0);
  driveRightB.move_voltage(0);
}

void Drivetrain::updateGyro(){
  gyroAngle = gyro.get_value();
}

void Drivetrain::updateSonics(){
  if(ultrasonicLeft.get_value() != -1){
		leftSonic = ultrasonicLeft.get_value();
	}
	if(ultrasonicRight.get_value() != -1){
		rightSonic = ultrasonicRight.get_value();
	}
}

void Drivetrain::driveLeft(int voltage){
  if(voltage > currentVoltageIndex){
    driveLeftF.move_voltage(currentVoltageIndex);
    driveLeftB.move_voltage(currentVoltageIndex);
  } else {
    driveLeftF.move_voltage(voltage);
    driveLeftB.move_voltage(voltage);
  }
}

void Drivetrain::driveRight(int voltage){
  if(voltage > currentVoltageIndex){
    driveRightF.move_voltage(currentVoltageIndex);
    driveRightB.move_voltage(currentVoltageIndex);
  } else {
    driveRightF.move_voltage(voltage);
    driveRightB.move_voltage(voltage);
  }
}

void Drivetrain::driveAll(int leftVoltage, int rightVoltage){
  if(driveState == 1){
    driveLeft(leftVoltage);
    driveRight(rightVoltage);
  } else if(driveState == -1){
   driveLeft(-rightVoltage);
   driveRight(-leftVoltage);
  }
}

void Drivetrain::toggleMaxSpeed(){
  if (toggleMaxSpeedButton.changedToPressed()) {
    if(currentVoltageIndex == 10000){
      currentVoltageIndex = 12000;
    } else if (currentVoltageIndex == 12000){
      currentVoltageIndex = 10000;
    }
  }
}

void Drivetrain::toggleDriveState(){
  if (driveReverseButton.changedToPressed()) {
    driveState = driveState * -1;
  }
}

bool Drivetrain::inRange(float low, float high, float x){
		return x < high && x > low;
}

//returns a vector of bools that are true if the ultrasonic is in the correct position ([0] for left and [1] for right)
//The driveRight goes to left ultrasonic because the drive direction is relative to the intake and the ultrasonic direction is relative to the forklift
std::vector<bool> Drivetrain::sonicDistanceAdjust(int leftDistance, int rightDistance) {
	updateSonics();
	bool leftSet = false;
	bool rightSet = false;

	//Equalization
	if((leftSonic < rightSonic-15 && leftDistance > leftSonic) || (leftSonic < rightSonic-15 && leftDistance < leftSonic)){
		driveRight(1800);
	  driveLeft(-1800);
	} else if ((rightSonic < leftSonic-15 && rightDistance > rightSonic) || (rightSonic < leftSonic-15 && rightDistance < rightSonic)){
		driveLeft(1800);
		driveRight(-1800);
	} else {
		//left adjustment
		if(leftDistance-10 < leftSonic && leftDistance+10 > leftSonic){
			driveRight(0);
			leftSet = true;
		}
		else if(leftDistance-10 > leftSonic){ //going forward (with respect ot the forklift being the front) means the 'speed' inputted must be negative
			driveRight(1800);
		}
		else if(leftDistance+10 < leftSonic){
			driveRight(-1800);
		}

		//right adjustment
		if(rightDistance-10 < rightSonic && rightDistance+10 > rightSonic){
			driveLeft(0);
			rightSet = true;
		}
		else if(rightDistance-10 > rightSonic){
			driveLeft(1800);
		}
		else if(rightDistance+10 < rightSonic){
			driveLeft(-1800);
		}
	}

	return std::vector<bool>{leftSet, rightSet};
}

//adjustDistance should be used where the move_voltagements are sequential and not simultanous, otherwise use sonicDistanceAdjust in the parent function
void Drivetrain::adjustDistance(int leftTarget, int rightTarget){
	bool completed = false;
	std::vector<bool> setSides;

  while(!completed){
		if (!inRange(-0.1, 0.1, controller.getAnalog(ControllerAnalog::leftY)) ||
				!inRange(-0.1, 0.1, controller.getAnalog(ControllerAnalog::rightY))) {
			completed = true; 	// User interrupt
		}
		setSides = sonicDistanceAdjust(leftTarget, rightTarget);
		if(setSides[0] == true && setSides[1] == true) {
			completed = true;
		}
    pros::delay(2);
	}
}

// //adjustDistance should be used where the moveVoltagements are sequential and not simultanous, otherwise use sonicDistanceAdjust in the parent function
// void Drivetrain::autoaim(){
// 	bool completed = false;
// 	std::vector<bool> setSides;
//
//   while(!completed){
// 		if (!inRange(-0.1, 0.1, controller.get_analog(ControllerAnalog::leftY)) ||
// 				!inRange(-0.1, 0.1, controller.get_analog(ControllerAnalog::rightY))) {
// 			completed = true; 	// User interrupt
// 		}
// 		setSides = sonicDistanceAdjust(leftTarget, rightTarget);
// 		if(setSides[0] == true && setSides[1] == true) {
// 			completed = true;
// 		}
//     pros::delay(2);
// 	}
// }

void Drivetrain::update(float leftVoltage, float rightVoltage){
  updateGyro();
  updateSonics();
  toggleMaxSpeed();
	toggleDriveState();

  float leftV =  leftVoltage * (float) currentVoltageIndex;
  float rightV = rightVoltage * (float) currentVoltageIndex;

	driveAll((int) leftV, (int) rightV);
}

void Drivetrain::turnWithGyro(int degrees){
  updateGyro();
  int tenthDegrees = degrees * 10;
  int targetDegrees = gyroAngle + tenthDegrees;

  int startMillis = pros::millis();
  int currentMillis = startMillis;
  bool completed = false;
  while(!completed){
    if((gyroAngle - targetDegrees) < 100){
      if(tenthDegrees < 0){
        driveLeft(-3000);
        driveRight(3000);
      } else {
        driveLeft(3000);
        driveRight(-3000);
      }
    } else {
      if(tenthDegrees < 0){
        driveLeft(-6000);
        driveRight(6000);
      } else {
        driveLeft(6000);
        driveRight(-6000);
      }
    }

    updateGyro();
    if(abs(gyroAngle - targetDegrees) < 20){
      completed = true;
    }

    currentMillis = pros::millis();
    if(currentMillis - startMillis > 100){
      completed = true;
    }

    pros::delay(2);
  }
  driveAll(0,0);
}

void Drivetrain::turnDegrees(int degrees){ //super janky needs a delay after it
  double wheelCircumference = 4.25 * 3.14159;
  double distanceMultiplier = degrees / 360;
  double robotArcCircumference = 14 * 3.14159; //14 is the width of the robot wheel to wheel, multiply by pi to get the circumference of the circle carved out
  double distanceNeeded = distanceMultiplier * robotArcCircumference;
  double tickMultiplier = distanceNeeded / wheelCircumference;
  int ticksNeededToTurn = ticksGreen * tickMultiplier;

  if(degrees < 0){
    driveLeftRelative(-ticksNeededToTurn, 150);
    driveRightRelative(ticksNeededToTurn, 150);
  } else {
    driveLeftRelative(ticksNeededToTurn, 150);
    driveRightRelative(-ticksNeededToTurn, 150);
  }
}

int Drivetrain::velocityBasedOnDistanceLeft(int ticksRemaining){
  int revolutionsRemaining = ticksRemaining / ticksGreen;
  double distanceRemaining = revolutionsRemaining * 4.25 * 3.14159;
  if (distanceRemaining <= 18.0) {
    return (distanceRemaining/18.0) * 200.0;
  } else {
    return 200.0;
  }
}

void Drivetrain::driveLeftRelative(int tickCount, int velocity){
  driveLeftF.move_relative(tickCount, velocity);
  driveLeftB.move_relative(tickCount, velocity);
}

void Drivetrain::driveRightRelative(int tickCount, int velocity){
  driveRightF.move_relative(tickCount, velocity);
  driveRightB.move_relative(tickCount, velocity);
}

void Drivetrain::driveDistance(double inches){
  double wheelCircumference = 4.25 * 3.14159;
  double tickMultiplier = inches / wheelCircumference;
  int ticksToMove = ticksGreen * tickMultiplier; //ticksgreen is the tiks per rev on 18:1 motor

  // int startLeftT = driveLeftB.get_position();
  // int startRightT = driveRightB.get_position();

  driveLeftRelative(ticksToMove, 200);
  driveRightRelative(ticksToMove, 200);

  // bool completed = false;
  // while(!completed){
  //   if((driveLeftB.get_position() == startLeftT + ticksToMove) &&
  //      (driveRightB.get_position() == startRightT + ticksToMove)){
  //     completed = true;
  //   }
  //
  //   pros::delay(3);
  // }

  // int leftTicksRemaining = ticksToMove;
  // int rightTicksRemaining = ticksToMove;
  //
  // int leftVelocity;
  // int rightVelocity;
  //
  // bool completed = false;
  // int startMillis = pros::millis();
  // int currentMillis = startMillis;
  // while(!completed){
  //   leftVelocity = velocityBasedOnDistanceLeft(leftTicksRemaining);
  //   rightVelocity = velocityBasedOnDistanceLeft(rightTicksRemaining);
  //
  //   driveLeftDistance(ticksToMove, leftVelocity);
  //   driveRightDistance(ticksToMove, rightVelocity);
  //
  //   leftTicksRemaining = driveLeftB.get_target_position() - driveLeftB.get_position();
  //   rightTicksRemaining = driveRightB.get_target_position() - driveRightB.get_position();
  //
  //   currentMillis = pros::millis();
  //   if(currentMillis - startMillis > 3000){
  //     completed = true;
  //   }
  //
  //   pros::delay(2);
  // }
  // driveAll(0,0);
}
