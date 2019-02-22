#include "eyes.h"
#include "main.h"
#include "robot.h"

// prints the signature to the terminal
void print_sig(pros::vision_signature_s_t signature){
    printf("(%d,%d,%d,%d,%d,%d,%d,%f,%d)",
    signature.id, signature.u_min, signature.u_max,
    signature.u_mean, signature.v_min, signature.v_max,
    signature.v_mean, signature.range, signature.type);
}

void Eyes::initialize(){
  // turn off wifi
  aiming_vision_sensor.set_wifi_mode(0);
  pole_vision_sensor.set_wifi_mode(0);

  // load signatures
  // aiming_vision_sensor.set_signature(const std::uint8_t signature_id, vision_signature_s_t *const signature_ptr);
  // ground_vision_sensor.set_signature(const std::uint8_t signature_id, vision_signature_s_t *const signature_ptr);

  // create color codes
  redflag = aiming_vision_sensor.create_color_code(1,3);
  blueflag = aiming_vision_sensor.create_color_code(2,3);
};

Eyes::Eyes()
:aiming_vision_sensor(AIMING_VISION_SENSOR, pros::E_VISION_ZERO_CENTER),
pole_vision_sensor(POLE_VISION_SENSOR, pros::E_VISION_ZERO_CENTER) {
  initialize();
};

void Eyes::call_autoaim(){
  if(autoaimButton.changedToPressed()){
    autoaim();
  }
}

// aligns the x position for now
void Eyes::autoaim(){
  Robot& robot = Robot::instance();

  // alliance color
  Alliance alliance = red;

  // aiming bias. + is right, - is left
  // this allows you to aim slightly right or left
  int bias = 0;

  // the object x coordinate
  int x_coord;

  pros::vision_object_s_t object;

  // check for errors
  // check for biggest object in the color code of alliance
  if(alliance == red){
    pros::vision_object_s_t object = aiming_vision_sensor.get_by_code(0, redflag);
    x_coord = object.left_coord + (object.width/2);
  }
  else if(alliance == blue){
    pros::vision_object_s_t object = aiming_vision_sensor.get_by_code(0, blueflag);
    x_coord = object.left_coord + (object.width/2);
  }
  pros::c::lcd_print(0, "object leftcoord, alliance: %d", object.left_coord, alliance);
  pros::c::lcd_print(7, "x coordinates: %d", x_coord);

  // while center coord is not within range of 5px from 0,
  while(!((x_coord <= 5) && (x_coord >= -5))){

    // controller interrupt handler
    if(!robot.drivetrain.inRange(-0.1, 0.1, controller.getAnalog(ControllerAnalog::leftY)) ||
				!robot.drivetrain.inRange(-0.1, 0.1, controller.getAnalog(ControllerAnalog::rightY))){
          break;
        }

    // redefine object, in case it disappears or changes
    if(alliance == red){
      pros::vision_object_s_t object = aiming_vision_sensor.get_by_code(0, redflag);
      x_coord = object.left_coord + (object.width/2);
    }
    else if(alliance == blue){
      pros::vision_object_s_t object = aiming_vision_sensor.get_by_code(0, blueflag);
      x_coord = object.left_coord + (object.width/2);
    }

    pros::c::lcd_print(0, "object leftcoord, alliance: %d", object.left_coord, alliance);
    pros::c::lcd_print(7, "x coordinates: %d", x_coord);

    // move according to x_coord
    // if x position > 0, set motors to move left
    if(x_coord>0){
      robot.drivetrain.driveAll(-3000,3000);
    }
    // elif x position > 0, set motors to move left
    else if(x_coord<0){
      robot.drivetrain.driveAll(3000,-3000);
    }

    pros::delay(20);
    robot.drivetrain.driveAll(0,0);
    // shoot - implement that later
  }
  // robot in position now
};

void Eyes::align_with_pole(){
  Robot& robot = Robot::instance();
};

void Eyes::update(){
  call_autoaim();
};
