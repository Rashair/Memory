#ifndef LINE_WRAPPER_H
#define LINE_WRAPPER_H

#include <iostream>

#include <gpiod.h>

class lineWrapper {
    gpiod_line* line;
    void (*releaseResources)();
    std::string consumer;

    gpiod_line* GetLine(gpiod_chip* chip, int num);

public:
    lineWrapper(
        gpiod_line* line, void (*releaseResources)() = []() {}, std::string consumer = "CON");
    lineWrapper(
        gpiod_chip* chip, int num, void (*releaseResources)() = []() {}, std::string consumer = "CON");

    int GetValue();

    void RequestBothEdgesEvents();

    int EventWait(timespec* ts);

    gpiod_line_event* EventRead();
};
#endif