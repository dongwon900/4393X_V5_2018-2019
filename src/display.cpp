#include "display.h"
#include "robot.h"

void Display::initialize(){
  alliance = Alliance::red;
  startingTile = StartingTile::back;
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
  pros::lcd::print(3, "limitToggled: %d", robot.intake.limitToggled);
  pros::lcd::print(4, "ballStaged: %d", robot.intake.ballStaged);
  pros::lcd::print(5, "intakeLimit: %d", robot.intake.intakeLimitValue);
}

void Display::displayDataOnController(){
  controller.clear();
  controller.setText(0, 0, "Auto 1");
  controller.setText(1, 0, "Auto 2");
  controller.setText(2, 0, "Auto 3");
}

bool Display::chooseAlliance(){
  pros::lcd::set_text(1, "Choose an Alliance");
  pros::lcd::set_text(3, "Left for Blue");
  pros::lcd::set_text(4, "Right for Red");
  if(pros::lcd::read_buttons() == 4){
    alliance = Alliance::blue;
    return true;
  } else if (pros::lcd::read_buttons() == 2){
    //lv_disp_flush(10, 50, 10, 50, COLOR_BLACK); //(teehee) I dug deep into the files and can now print images to the touchscreen
    return false;
  } else if (pros::lcd::read_buttons() == 1){
    alliance = Alliance::red;
    return true;
  } else {
    return false;
  }
}

bool Display::chooseTile(){
  pros::lcd::set_text(1, "Choose a Tile");
  pros::lcd::set_text(3, "Left for Front");
  pros::lcd::set_text(4, "Right for Back");
  if(pros::lcd::read_buttons() == 4){
    startingTile = StartingTile::front;
    return true;
  } else if (pros::lcd::read_buttons() == 2){
    //lv_disp_flush(10, 50, 10, 50, COLOR_BLACK); //(teehee) I dug deep into the files and can now print images to the touchscreen
    return false;
  } else if (pros::lcd::read_buttons() == 1){
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
    printf("%s\n", "Alliance Selector");
    pros::delay(10);
  }
  pros::lcd::clear();
  pros::lcd::set_text(1, "The inbetween");
  pros::delay(1000);
  while(!tileSelected){
    tileSelected = chooseTile();
    printf("%s\n", "Tile Selector");
    pros::delay(10);
  }

  pros::lcd::clear();
  printf("%s\n", "Cleared screen");
  if(alliance == Alliance::red){
    pros::lcd::set_text(1, "Red Alliance");
    printf("%s\n", "Red Alliance");
  } else {
    pros::lcd::set_text(1, "Blue Alliance");
    printf("%s\n", "Blue Alliance");
  }
  if(startingTile == StartingTile::front){
    pros::lcd::set_text(2, "Front Tile");
    printf("%s\n", "Front Tile");
  } else {
    pros::lcd::set_text(2, "Back Tile");
    printf("%s\n", "Back Tile");
  }
}

void Display::update(){
  displaySensorValuesOnBrain();
  displayDataOnController();

  //printf("%s\n", "I am running");
}

Alliance Display::getAlliance(){
  return alliance;
}

StartingTile Display::getStartingTile(){
  return startingTile;
}
