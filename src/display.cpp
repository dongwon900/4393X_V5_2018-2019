#include "display.h"
#include "smartController.h"

Display::Display(){

}

Display::~Display(){

}

/*
void Display::displaySensorValuesOnBrain(){
  pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
                   (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
                   (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);
  pros::lcd::print(1, "Ultrasonic left: %d", leftSonic);
  pros::lcd::print(2, "Ultrasonic right: %d", rightSonic);
  pros::lcd::print(3, "Gyro: %d", gyroAngle);
  pros::lcd::print(4, "Vision 1: %d", gyroAngle);
  pros::lcd::print(5, "Vision 2: %d", gyroAngle);
  pros::lcd::print(6, "Vision 3: %d", gyroAngle);
  pros::lcd::print(7, "Vision 4: %d", gyroAngle);
}
*/

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

  printf("%s\n", "I am running");
}
