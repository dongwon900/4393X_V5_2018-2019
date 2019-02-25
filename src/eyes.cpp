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

  aiming_vision_sensor.set_exposure(19);

  red_sig = aiming_vision_sensor.signature_from_utility(1, 7811, 8475, 8143, -347, 187, -80, 11.000, 1);
  blue_sig = aiming_vision_sensor.signature_from_utility(2, -3369, -2545, -2958, 10127, 11227, 10676, 8.300, 1);
  green_sig = aiming_vision_sensor.signature_from_utility(3, -2427, -2009, -2218, -3959, -3489, -3724, 6.800, 1);

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
  // setup boi -- "initialization"
  Robot& robot = Robot::instance();
  // alliance color
  Alliance alliance = robot.display.getAlliance();
  // aiming bias. + is right, - is left
  // this allows you to aim slightly right or left
  int bias = 3;
  // the object x coordinate
  int x_coord = 10;
  // the object to save to
  pros::vision_object_s_t object;

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
      x_coord = object.x_middle_coord;
    }
    else if(alliance == blue){
      pros::vision_object_s_t object = aiming_vision_sensor.get_by_code(0, blueflag);
      x_coord = object.x_middle_coord;
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
