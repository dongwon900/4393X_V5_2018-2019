#include "main.h"
#include "definitions.h"
#include "robot.h"

void autoRedFront1(){
  Robot& robot = Robot::instance();

  robot.forklift.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
  robot.forklift.moveVelocity(-200);
  pros::delay(2200);
  robot.forklift.moveVelocity(0);
  robot.drivetrain.driveDistance(-42);
  robot.forklift.moveVelocity(200); //raises the fork to pickup the cap
  pros::delay(550); //waits for this action to complete
  robot.forklift.moveVelocity(0); //stops the forklift motor
  robot.drivetrain.driveDistance(46);
  robot.drivetrain.driveDistance(-10);
  robot.drivetrain.turnDegrees(-90);
  robot.drivetrain.driveDistance(14);
  robot.drivetrain.turnDegrees(-90);
  robot.lift.raiseLiftIndex();
  robot.lift.raiseLiftIndex();
  robot.lift.raiseLiftIndex();
  while(robot.lift.liftSet == false){
    robot.lift.update();
    pros::delay(5);
  }
  robot.drivetrain.adjustDistance(200, 200);
  robot.lift.lowerLiftIndex();
  while(robot.lift.liftSet == false){
    robot.lift.update();
    pros::delay(5);
  }
  robot.drivetrain.driveDistance(12);
  robot.lift.lowerLiftIndex();
  robot.lift.lowerLiftIndex();
  while(robot.lift.liftSet == false){
    robot.lift.update();
    pros::delay(5);
  }
  robot.drivetrain.turnDegrees(-100);
  robot.drivetrain.driveDistance(40);
}

void autoRedBack1(){
  Robot& robot = Robot::instance();

  robot.forklift.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
  robot.forklift.moveVelocity(-200);
  pros::delay(2200);
  robot.forklift.moveVelocity(0);
  robot.drivetrain.driveDistance(-42); //drives to the blue tilted cap
  pros::delay(250);
  robot.forklift.moveVelocity(200); //raises the fork to pickup the cap
  pros::delay(750); //waits for this action to complete
  robot.forklift.moveVelocity(0); //stops the forklift motor
  robot.drivetrain.turnDegrees(90); //turns to face the platforms
  robot.drivetrain.driveDistance(12); //drives into the platforms to level off
  robot.drivetrain.driveDistance(-11); //drives away from the platforms
  robot.drivetrain.turnDegrees(90); //turns to face the pole
  robot.drivetrain.driveDistance(-24); //drives towards the pole
  robot.lift.raiseLiftIndex();
  robot.lift.raiseLiftIndex(); //raises the lift
  robot.lift.liftSet = false;
  while(robot.lift.liftSet == false){
    robot.lift.update();
    pros::delay(5);
  }
  robot.forklift.moveVelocity(-100);
  pros::delay(110);
  robot.forklift.moveVelocity(0);
  robot.drivetrain.adjustDistance(200, 200); //aligns with the pole and drives forward
  robot.drivetrain.driveDistance(-6); //jolts back to the pole incase it backedup
  robot.lift.lowerLiftIndex();
  robot.lift.liftSet = false;
  while(robot.lift.liftSet == false){
    robot.lift.update();
    pros::delay(5);
  }
  robot.drivetrain.driveDistance(30);
  robot.lift.lowerLiftIndex();
  robot.lift.liftSet = false;
  while(robot.lift.liftSet == false){
    robot.lift.update();
    pros::delay(5);
  }
  robot.forklift.moveVelocity(-200);
  pros::delay(250);
  robot.forklift.moveVelocity(0);
  robot.drivetrain.turnDegrees(-90);
  robot.drivetrain.driveDistance(20);
  robot.drivetrain.driveDistance(-20);
  robot.drivetrain.turnDegrees(-90);
  robot.drivetrain.driveDistance(14);
  robot.drivetrain.driveDistance(-15);
  robot.forklift.moveVelocity(200);
  robot.drivetrain.driveDistance(-16);
  robot.forklift.moveVelocity(0);
}

void autoBlueFront1(){
  Robot& robot = Robot::instance();

  robot.forklift.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
  robot.forklift.moveVelocity(-200);
  pros::delay(2200);
  robot.forklift.moveVelocity(0);
  robot.drivetrain.driveDistance(-42);
  robot.forklift.moveVelocity(200); //raises the fork to pickup the cap
  pros::delay(550); //waits for this action to complete
  robot.forklift.moveVelocity(0); //stops the forklift motor
  robot.drivetrain.driveDistance(46);
  robot.drivetrain.driveDistance(-10);
  robot.drivetrain.turnDegrees(90);
  robot.drivetrain.driveDistance(14);
  robot.drivetrain.turnDegrees(90);
  robot.lift.raiseLiftIndex();
  robot.lift.raiseLiftIndex();
  robot.lift.raiseLiftIndex();
  while(robot.lift.liftSet == false){
    robot.lift.update();
    pros::delay(5);
  }
  robot.drivetrain.adjustDistance(200, 200);
  robot.lift.lowerLiftIndex();
  while(robot.lift.liftSet == false){
    robot.lift.update();
    pros::delay(5);
  }
  robot.drivetrain.driveDistance(12);
  robot.lift.lowerLiftIndex();
  robot.lift.lowerLiftIndex();
  while(robot.lift.liftSet == false){
    robot.lift.update();
    pros::delay(5);
  }
  robot.drivetrain.turnDegrees(100);
  robot.drivetrain.driveDistance(40);
}

void autoBlueBack1(){
  Robot& robot = Robot::instance();

  robot.forklift.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
  robot.forklift.moveVelocity(-200);
  pros::delay(2200);
  robot.forklift.moveVelocity(0);
  robot.drivetrain.driveDistance(-42); //drives to the blue tilted cap
  pros::delay(250);
  robot.forklift.moveVelocity(200); //raises the fork to pickup the cap
  pros::delay(750); //waits for this action to complete
  robot.forklift.moveVelocity(0); //stops the forklift motor
  robot.drivetrain.turnDegrees(-90); //turns to face the platforms
  robot.drivetrain.driveDistance(12); //drives into the platforms to level off
  robot.drivetrain.driveDistance(-11); //drives away from the platforms
  robot.drivetrain.turnDegrees(-90); //turns to face the pole
  robot.drivetrain.driveDistance(-24); //drives towards the pole
  robot.lift.raiseLiftIndex();
  robot.lift.raiseLiftIndex(); //raises the lift
  robot.lift.liftSet = false;
  while(robot.lift.liftSet == false){
    robot.lift.update();
    pros::delay(5);
  }
  robot.forklift.moveVelocity(-100);
  pros::delay(110);
  robot.forklift.moveVelocity(0);
  robot.drivetrain.adjustDistance(200, 200); //aligns with the pole and drives forward
  robot.drivetrain.driveDistance(-6); //jolts back to the pole incase it backedup
  robot.lift.lowerLiftIndex();
  robot.lift.liftSet = false;
  while(robot.lift.liftSet == false){
    robot.lift.update();
    pros::delay(5);
  }
  robot.drivetrain.driveDistance(30);
  robot.lift.lowerLiftIndex();
  robot.lift.liftSet = false;
  while(robot.lift.liftSet == false){
    robot.lift.update();
    pros::delay(5);
  }
  robot.forklift.moveVelocity(-200);
  pros::delay(250);
  robot.forklift.moveVelocity(0);
  robot.drivetrain.turnDegrees(90);
  robot.drivetrain.driveDistance(20);
  robot.drivetrain.driveDistance(-20);
  robot.drivetrain.turnDegrees(90);
  robot.drivetrain.driveDistance(14);
  robot.drivetrain.driveDistance(-15);
  robot.forklift.moveVelocity(200);
  robot.drivetrain.driveDistance(-16);
  robot.forklift.moveVelocity(0);
}

void autonomous() {
 Robot& robot = Robot::instance();

  Alliance alliance = robot.display.getAlliance();
  StartingTile startingTile = robot.display.getStartingTile();

  switch((int)alliance){
    case 1:
      switch((int)startingTile){
        case 1:
          autoRedFront1();
          break;
        case 2:
          autoRedBack1();
          break;
      }
      break;
    case 2:
      switch((int)startingTile){
        case 1:
          autoBlueFront1();
          break;
        case 2:
          autoBlueBack1();
          break;
      }
      break;
  }
  for(unsigned int i = 0; i < 5; i++){ //helps to settle the robot after autonomous
    pros::delay(50);
  }
}
