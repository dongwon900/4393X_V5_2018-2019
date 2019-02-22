#include "drivetrain.h"

void Drivetrain::initialize(){
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
  performingMovement = false;
  performingAutoFunction = false;
  currentMillis = pros::millis();
  targetMillis = pros::millis();
}

Drivetrain::Drivetrain()
:driveLeftF(DRIVETRAIN_L_F),
driveLeftB(DRIVETRAIN_L_B),
driveRightF(-DRIVETRAIN_R_F),
driveRightB(-DRIVETRAIN_R_B),
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
		driveRight(2700);
	  driveLeft(-2700);
	} else if ((rightSonic < leftSonic-15 && rightDistance > rightSonic) || (rightSonic < leftSonic-15 && rightDistance < rightSonic)){
		driveLeft(2700);
		driveRight(-2700);
	} else {
		//left adjustment
		if(leftDistance-10 < leftSonic && leftDistance+10 > leftSonic){
			driveRight(0);
			leftSet = true;
		}
		else if(leftDistance-10 > leftSonic){ //going forward (with respect ot the forklift being the front) means the 'speed' inputted must be negative
			driveRight(4000);
		}
		else if(leftDistance+10 < leftSonic){
			driveRight(-4000);
		}

		//right adjustment
		if(rightDistance-10 < rightSonic && rightDistance+10 > rightSonic){
			driveLeft(0);
			rightSet = true;
		}
		else if(rightDistance-10 > rightSonic){
			driveLeft(4000);
		}
		else if(rightDistance+10 < rightSonic){
			driveLeft(-4000);
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

void Drivetrain::update(float leftVoltage, float rightVoltage){
  updateSonics();
  toggleMaxSpeed();
	toggleDriveState();

  float leftV =  leftVoltage * (float) currentVoltageIndex;
  float rightV = rightVoltage * (float) currentVoltageIndex;

	driveAll((int) leftV, (int) rightV);
}

void Drivetrain::driveLeftRelative(int tickCount, int velocity){
  driveLeftF.move_relative(tickCount, velocity);
  driveLeftB.move_relative(tickCount, velocity);
}

void Drivetrain::driveRightRelative(int tickCount, int velocity){
  driveRightF.move_relative(tickCount, velocity);
  driveRightB.move_relative(tickCount, velocity);
}

void Drivetrain::driveAllRelative(int leftTickCount, int rightTickCount, int leftVelocity, int rightVelocity){
  driveLeftRelative(leftTickCount, leftVelocity);
  driveRightRelative(rightTickCount, rightVelocity);
}

void Drivetrain::driveLeftVelocity(int velocity){
  driveLeftF.move_velocity(velocity);
  driveLeftB.move_velocity(velocity);
}

void Drivetrain::driveRightVelocity(int velocity){
  driveRightF.move_velocity(velocity);
  driveRightB.move_velocity(velocity);
}

void Drivetrain::driveAllVelocity(int leftVelocity, int rightVelocity){
  driveLeftVelocity(leftVelocity);
  driveRightVelocity(rightVelocity);
}

void Drivetrain::setBrakeMode(pros::motor_brake_mode_e_t mode){
  driveLeftF.set_brake_mode(mode);
  driveLeftB.set_brake_mode(mode);
  driveRightF.set_brake_mode(mode);
  driveRightB.set_brake_mode(mode);
}

int Drivetrain::velocityBasedOnDistanceLeft(int ticksRemaining){
  double revolutionsRemaining = ticksRemaining / ticksGreen;
  double distanceRemaining = revolutionsRemaining * 4.25 * 3.14159;
  if (distanceRemaining <= 18.0) {
    if((distanceRemaining/18.0) * 200.0 > 20){
      return (distanceRemaining/18.0) * 200.0;
    } else {
      return 20;
    }
  } else {
    return 200.0;
  }
}

//returns the millis it takes to complete
//delayRatio is a multipier
void Drivetrain::turnDegrees(double degrees, double delayRatio){
  double tickMultiplier = degrees / 360;
  double ticksToMove = tickMultiplier * ticksGreen * 1.5;

  driveAllRelative(ticksToMove, -ticksToMove, 200, 200);

  double delay = abs((double)ticksToMove * delayRatio);
  pros::delay((int)delay);

  if(degrees > 0){
    driveAll(-12000, 12000);
  } else {
    driveAll(12000, -12000);
  }
  pros::delay(30);
  driveAll(0,0);
}

//returns the millis it takes to complete
//delayRatio is a divisor
//common delayRatio vallues, 6 inches 1.8, 12 inches 2.15
void Drivetrain::driveDistance(double inches, double delayRatio){
  double wheelCircumference = 4.25 * 3.14159;
  double tickMultiplier = inches / wheelCircumference;
  int ticksToMove = ticksGreen * tickMultiplier; //ticksgreen is the tiks per rev on 18:1 motor

  driveAllRelative(ticksToMove, ticksToMove, 150, 150);

  double delay = abs((double)ticksToMove / delayRatio);
  pros::delay((int)delay);

  if(inches > 0){
    driveAll(-12000, -12000);
  } else {
    driveAll(12000, 12000);
  }
  pros::delay(85);
  driveAll(0,0);
  pros::delay(20);
}

//using 1.33 + 0.289 ln x to determine the delayRatio, this was found through compliling the experimental values and fitting a trendline
void Drivetrain::driveDistance(double inches){
  double delayRatio = 1.33 + 0.289 * log(abs(inches));
  driveDistance(inches, delayRatio);
}

//using 4.74 - 0.397 ln (degrees) to determine the delayRatio, this was found by fitting a curve
void Drivetrain::turnDegrees(double degrees){
  double delayRatio = 4.74 - 0.397 * log(abs(degrees));
  turnDegrees(degrees, delayRatio);
}

//takes a sign either 1 or -1 only to determine the diretion. 1 is 45 to the right, -1 to the left
void Drivetrain::turn45(int sign){
  if(sign == 1){
    turnDegrees(45, 3.2);
  } else if(sign == -1){
    turnDegrees(-45, 3.2);
  }
}

//takes a sign either 1 or -1 only to determine the direction. 1 is 90 to the right, -1 to the left
void Drivetrain::turn90(int sign){
  if(sign == 1){
    turnDegrees(90, 3);
  } else if(sign == -1){
    turnDegrees(-90, 3);
  }
}

//takes a sign either 1 or -1 only to determine the direction. 1 is 180 to the right, -1 to the left
void Drivetrain::turn180(int sign){
  if(sign == 1){
    turnDegrees(180, 2.65);
  } else {
    turnDegrees(-180, 2.65);
  }
}
