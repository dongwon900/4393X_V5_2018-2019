#ifndef _EYES_H_
#define _EYES_H_

#include "main.h"
#include "definitions.h"
#include <vector>

// prints a signautre's parameters to the terminal
void print_sig(
  int32_t id, int32_t u_min, int32_t u_max,
  int32_t u_mean, int32_t v_min, int32_t v_max,
  int32_t v_mean, float range, int32_t type);

class Eyes{
public:
  // vision sensor on the lift upper arm
  Vision aiming_vision_sensor;
  // vision sensor for the flags
  Vision pole_vision_sensor;
  Eyes();
  void initialize();
  void autoaim();
  void align_with_pole();
  void update();
  void call_autoaim();

  pros::vision_signature_s_t red_sig;
  pros::vision_signature_s_t blue_sig;
  pros::vision_signature_s_t green_sig;

  pros::vision_color_code_t redflag;
  pros::vision_color_code_t blueflag;
};

#endif
