
#include <string.h>

#include "lineWrapper.hpp"
#include "utility.hpp"

gpiod_line* lineWrapper::GetLine(gpiod_chip* chip, int num)
{
    auto line = gpiod_chip_get_line(chip, num);
    if (line == nullptr) {
        releaseResources();
        ERR("Get line failed\n");
    }

    return line;
}

lineWrapper::lineWrapper(gpiod_line* line, void (*releaseResources)(), std::string consumer)
{
    this->line = line;
    this->releaseResources = releaseResources;
    this->consumer = consumer;
}

lineWrapper::lineWrapper(gpiod_chip* chip, int num, void (*releaseResources)(), std::string consumer)
    : lineWrapper(GetLine(chip, num), releaseResources, consumer)
{
}

int lineWrapper::GetValue()
{
    int val = gpiod_line_get_value(line);
    if (val == -1) {
        std::cerr << "Oh dear, something went wrong with get_value: " << strerror(errno) << "\n";
        errno = 0;
    }

    return val;
}

void lineWrapper::RequestBothEdgesEvents()
{
    int val = gpiod_line_request_both_edges_events(line, consumer.c_str());
    if (val < 0) {
        releaseResources();
        ERR("Request event notification failed\n");
    }
}

int lineWrapper::EventWait(timespec* ts)
{
    int val = gpiod_line_event_wait(line, ts);
    if (val < 0) {
        releaseResources();
        ERR("Event notification failed\n");
    }

    return val;
}

gpiod_line_event* lineWrapper::EventRead()
{
    auto event = new gpiod_line_event();
    int val = gpiod_line_event_read(line, event);
    if (val < 0) {
        releaseResources();
        ERR("Read last event notification failed\n");
    }

    return event;
}
