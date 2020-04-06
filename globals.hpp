#ifndef GLOBALS_H
#define GLOBALS_H
#include <gpiod.h>

namespace resources {

extern bool isDebugOn;

extern const char* consumer;
extern const int bounceMsWait;

extern const char* chipName;
extern gpiod_chip* chip;

extern int buttonsStart;
extern int buttonsCount;

extern int diodesStart;
extern int diodesCount;

} // namespace resources

#endif