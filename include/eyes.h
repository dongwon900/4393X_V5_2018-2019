#include "main.h"
#include "definitions.h"
#include <vector>

extern Vision visionSensor1;
// vision sensor on the midsection
extern Vision visionSensor2;

class eyes{
private:
  pros::vision_object_s_t closest_ball;
  pros::vision_object_s_t closest_cap;
public:
  eyes();

  // finds nearest ball and returns the lateral location
  std::vector<int16_t> find_nearest_ball();

  // finds nearest cap and returns the lateral location
  std::vector<int16_t> find_nearest_cap();

  // finds nearest ball and returns the lateral location
  std::vector<std::vector<int16_t>> findflags(uint32_t signature);

  std::string return_cap_color();

  // updates the vision object fields
  void update_visionobjects(pros::vision_object_s_t closest_ball, pros::vision_object_s_t closest_cap);
  // gets the vision object fields
  void get_visionobjects();

};
