#ifndef GLOBALS_H
#define GLOBALS_H
#include <gpiod.h>

namespace resources {

extern bool isDebugOn;

extern const char* consumer;
extern const int bounceMsWait;

extern const char* bName;
extern gpiod_chip* buttonChip;
extern int buttonsCount;
extern int buttonsStart;
extern gpiod_line_bulk* buttons;

extern int diodesStart;

} // namespace resources

#endif