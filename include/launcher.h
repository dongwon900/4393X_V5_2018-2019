#include "main.h"

extern Motor launcherMotor;

class Launcher{
private:
  Motor launcherMotor;
  int launcherLimit;
public:
  Launcher();
  ~Launcher();
  void launcherAutoMovement();
  void update();
};