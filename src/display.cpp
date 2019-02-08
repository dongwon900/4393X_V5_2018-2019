#include "display.h"
#include "robot.h"

void Display::initialize(){

}

Display::Display(){
  initialize();
}

Display::~Display(){

}

void Display::displaySensorValuesOnBrain(){
  Robot& robot = Robot::instance();
  pros::lcd::print(1, "Ultrasonic left: %d", robot.drivetrain.leftSonic);
  pros::lcd::print(2, "Ultrasonic right: %d", robot.drivetrain.rightSonic);
  pros::lcd::print(3, "Gyro: %d", robot.drivetrain.gyroAngle);
}

void Display::displayDataOnController(){
  controller.clear();
  controller.setText(0, 0, "Auto 1");
  controller.setText(1, 0, "Auto 2");
  controller.setText(2, 0, "Auto 3");
}

bool Display::chooseAlliance(){
  pros::lcd::clear();
  pros::lcd::set_text(1, "Choose an Alliance");
  pros::lcd::set_text(3, "Left for Blue");
  pros::lcd::set_text(4, "Right for Red");
  if(pros::lcd::read_buttons() == 4){
    alliance = Alliance::blue;
    return true;
  } else if (pros::lcd::read_buttons() == 2){
    //lv_disp_flush(10, 50, 10, 50, COLOR_BLACK); //(teehee) I dug deep into the files and can now print images to the touchscreen
    return false;
  } else if (pros::lcd::read_buttons() == 2){
    alliance = Alliance::red;
    return true;
  } else {
    return false;
  }
}

bool Display::chooseTile(){
  pros::lcd::clear();
  pros::lcd::set_text(1, "Choose a Tile");
  pros::lcd::set_text(3, "Left for Front");
  pros::lcd::set_text(4, "Right for Back");
  if(pros::lcd::read_buttons() == 4){
    startingTile = StartingTile::front;
    return true;
  } else if (pros::lcd::read_buttons() == 2){
    //lv_disp_flush(10, 50, 10, 50, COLOR_BLACK); //(teehee) I dug deep into the files and can now print images to the touchscreen
    return false;
  } else if (pros::lcd::read_buttons() == 2){
    startingTile = StartingTile::back;
    return true;
  } else {
    return false;
  }
}

void Display::chooseAuto(){
  bool allianceSelected = false;
  bool tileSelected = false;

  while(!allianceSelected){
    allianceSelected = chooseAlliance();
    pros::delay(2);
  }
  while(!tileSelected){
    tileSelected = chooseTile();
    pros::delay(2);
  }
}

void Display::update(){
  //displaySensorValuesOnBrain();
  displayDataOnController();

  //printf("%s\n", "I am running");
}
