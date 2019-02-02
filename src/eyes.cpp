#include "eyes.h"
#include "main.h"
#include "robot.h"

// vision sensor on the lift upper arm
Vision visionSensor1(VISION_1, pros::E_VISION_ZERO_CENTER);
// vision sensor on the midsection
Vision visionSensor2(VISION_2, pros::E_VISION_ZERO_CENTER);

Vision visionSensor3(VISION_3, pros::E_VISION_ZERO_CENTER);
Vision visionSensor4(VISION_4, pros::E_VISION_ZERO_CENTER);

eyes::eyes(){

};

std::vector<int16_t> eyes::find_nearest_ball(){
  // gets the biggest ball in front
  pros::vision_object_s_t ball = visionSensor2.get_by_sig(0,3);
  std::vector<int16_t> location_vector = {ball.x_middle_coord, ball.y_middle_coord};
  return location_vector;
};

std::vector<int16_t> eyes::find_nearest_cap(){
  // gets the biggest cap in front
  pros::vision_object_s_t cap = visionSensor2.get_by_size(0);
  std::vector<int16_t> location_vector;

  // if cap is red
  if(cap.signature == RED_SIG){
    location_vector = {cap.x_middle_coord, cap.y_middle_coord, RED_SIG};
  }
  // if cap is blue
  else if(cap.signature == BLUE_SIG){
    location_vector = {cap.x_middle_coord, cap.y_middle_coord, BLUE_SIG};
  }
  return location_vector;
};

std::string eyes::return_cap_color(){
  std::string color;
  if(find_nearest_cap()[2] == RED_SIG){
    color = "RED";
  }
  // if cap is blue
  else if(find_nearest_cap()[2] == BLUE_SIG){
    color = "BLUE";
  }
  return color;
};

// std::vector<std::vector<int16_t>> findflags(uint32_t signature){
//
//   pros::vision_object_s_t flag1 = visionSensor1.get_by_sig(0, signature);
//   pros::vision_object_s_t flag2 = visionSensor1.get_by_sig(1, signature);
//   pros::vision_object_s_t flag3 = visionSensor1.get_by_sig(2, signature);
//   std::vector<int16_t> location_vector = {flag1.x_middle_coord, flag1.y_middle_coord};
//   return location_vector;
// };


void update_visionobjects(pros::vision_object_s_t closest_ball, pros::vision_object_s_t closest_cap){

};

void get_visionobjects(){

};
