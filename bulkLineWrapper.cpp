
#include <cstring>
#include <iostream>

#include "bulkLineWrapper.hpp"
#include "utility.hpp"

gpiod_line_bulk* bulkLineWrapper::getLines(gpiod_chip* chip, unsigned* offsets, int count)
{
    gpiod_line_bulk* result = new gpiod_line_bulk();
    int val = gpiod_chip_get_lines(chip, offsets, count, result);
    if (val < 0) {
        releaseResources();
        ERR("Get lines failed\n");
    }

    return result;
}

bulkLineWrapper::bulkLineWrapper(gpiod_line_bulk* lines, void (*releaseResources)(), std::string consumer)
{
    this->lines = lines;
    this->count = gpiod_line_bulk_num_lines(lines);
    this->releaseResources = releaseResources;
    this->consumer = consumer;
}

bulkLineWrapper::bulkLineWrapper(gpiod_chip* chip, unsigned* offsets, int offsetsCount, void (*releaseResources)(), std::string consumer)
    : bulkLineWrapper(getLines(chip, offsets, offsetsCount), releaseResources, consumer)
{
}

bulkLineWrapper::~bulkLineWrapper()
{
    delete lines;
};

std::shared_ptr<int> bulkLineWrapper::getValues()
{
    std::shared_ptr<int> values(new int[count]);
    int val = gpiod_line_get_value_bulk(lines, values.get());
    if (val == -1) {
        std::cerr << "Oh dear, something went wrong with get_value: " << strerror(errno) << "\n";
        errno = 0;
    }

    return values;
}

void bulkLineWrapper::requestFallingEdgeEvents()
{
    int val = gpiod_line_request_bulk_falling_edge_events(lines, consumer.c_str());
    if (val < 0) {
        releaseResources();
        ERR("Request events notification failed\n");
    }
}

void bulkLineWrapper::requestBothEdgesEvents()
{
    int val = gpiod_line_request_bulk_both_edges_events(lines, consumer.c_str());
    if (val < 0) {
        releaseResources();
        ERR("Request events notification failed\n");
    }
}

std::shared_ptr<gpiod_line_bulk> bulkLineWrapper::waitForEvent(timespec* ts)
{
    std::shared_ptr<gpiod_line_bulk> events(new gpiod_line_bulk());
    int val = gpiod_line_event_wait_bulk(lines, ts, events.get());
    if (val < 0) {
        releaseResources();
        ERR("Event notification failed\n");
    }

    return val > 0 ? events : nullptr;
}

std::shared_ptr<gpiod_line_event> bulkLineWrapper::readEvents(gpiod_line_bulk* eventLines)
{
    int evCount = gpiod_line_bulk_num_lines(eventLines);
    std::shared_ptr<gpiod_line_event> events(new gpiod_line_event[evCount]);
    for (unsigned i = 0; i < eventLines->num_lines; ++i) {
        auto line = eventLines->lines[i];
        gpiod_line_event_read(line, &(events.get()[i]));

        int offset = gpiod_line_offset(line);
        debug("Event: " + std::to_string(offset));
    }

    return events;
}
