#pragma once

#include <iostream>
#include <memory>
#include <unistd.h>

#include <gpiod.h>

class bulkLineWrapper {
    int count;
    gpiod_line_bulk* lines;
    void (*releaseResources)();
    const char* consumer;

    gpiod_line_bulk* getLines(gpiod_chip* chip, unsigned* offsets, int count);

public:
    bulkLineWrapper(
        gpiod_line_bulk* lines, void (*releaseResources)() = []() {}, std::string consumer = "CON");
    bulkLineWrapper(
        gpiod_chip* chip, unsigned* offsets, int offsetsCount, void (*releaseResources)() = []() {}, std::string consumer = "CON");

    ~bulkLineWrapper();

    std::shared_ptr<int> getValues();

    void setValue(int lineNum, int value);

    void blink(int lineNum, int ms);

    void requestOutput();

    void requestFallingEdgeEvents();

    void requestBothEdgesEvents();

    std::shared_ptr<gpiod_line_bulk> waitForEvent(timespec* ts);

    std::shared_ptr<gpiod_line_event> readEvent(int lineNum);

    std::shared_ptr<gpiod_line_event> readEvents(gpiod_line_bulk* eventLines);
};
