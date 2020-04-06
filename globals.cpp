#include "globals.hpp"

namespace resources {

bool isDebugOn = false;

const char* consumer = "CON";
const int bounceMsWait = 200;

const char* chipName = "gpiochip1";
gpiod_chip* chip;

int buttonsStart = 12;
int buttonsCount = 3;

int diodesStart = 24;
int diodesCount = buttonsCount + 1;

} // namespace resources
