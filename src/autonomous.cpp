#include "main.h"
#include "definitions.h"
#include "robot.h"

void autoRedFront1(){
  Robot& robot = Robot::instance();

  robot.drivetrain.turnDegrees(45);
}

void autoRedBack1(){
  Robot& robot = Robot::instance();

  robot.drivetrain.turnDegrees(-45);
}

void autoBlueFront1(){
  Robot& robot = Robot::instance();

  robot.drivetrain.driveDistance(6);
}

void autoBlueBack1(){
  Robot& robot = Robot::instance();

  robot.drivetrain.driveDistance(-6);
}

void autonomous() {
 Robot& robot = Robot::instance();

  Alliance alliance = robot.display.getAlliance();
  StartingTile startingTile = robot.display.getStartingTile();

  printf("%i\n", (int)alliance);
  printf("%i\n", (int)startingTile);

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

  robot.initialize();
}
