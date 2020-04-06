#include "globals.hpp"

namespace resources {

bool isDebugOn = false;

const char* consumer = "CON";
const int bounceMsWait = 200;

const char* bName = "gpiochip1";
gpiod_chip* buttonChip;
int buttonsCount = 3;
int buttonsStart = 12;
gpiod_line_bulk* buttons;

int diodesStart = 24;

} // namespace resources
