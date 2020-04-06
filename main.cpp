#include <iostream>

#include <cerrno>
#include <csignal>
#include <cstring>
#include <gpiod.h>
#include <unistd.h>

#include "bulkLineWrapper.hpp"
#include "globals.hpp"
#include "utility.hpp"

using namespace std;
using namespace resources;

void releaseResources()
{
    gpiod_line_release_bulk(buttons);
    gpiod_chip_close(buttonChip);
}

gpiod_chip* getChipByName(const char* cName)
{
    gpiod_chip* chip = gpiod_chip_open_by_name(cName);
    if (chip == nullptr) {
        ERR("Open chip failed");
    }

    return chip;
}

void handleArgs(int argc, char* argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, "db:c:")) != -1) {
        switch (opt) {
            case 'b': {
                int val = strtol(optarg, nullptr, 10);
                if (val <= 0 || errno == ERANGE) {
                    err("Invalid argument: buttonsStart=" + string(optarg));
                    usage(argv[0]);
                    errno = 0;
                }
                else {
                    buttonsStart = val;
                    cout << "Buttons-start: " << buttonsStart << "\n";
                }
                break;
            }
            case 'i': {
                int val = strtol(optarg, nullptr, 10);
                if (val <= 0 || errno == ERANGE) {
                    err("Invalid argument: diodsStart=" + string(optarg));
                    usage(argv[0]);
                    errno = 0;
                }
                else {
                    diodesStart = val;
                }
                break;
            }
            case 'c': {
                int val = strtol(optarg, nullptr, 10);
                if (val <= 0 || errno == ERANGE) {
                    err("Invalid argument: buttonsCount=" + string(optarg));
                    usage(argv[0]);
                    errno = 0;
                }
                else {
                    buttonsCount = val;
                }
                break;
            }
            case 'd': {
                cout << "Debugging mode on.\n";
                isDebugOn = true;
                break;
            }
            case '?':
                err("Invalid argument");
                usage(argv[0]);
                break;
        }
    }
}

int main(int argc, char* argv[])
{
    // ios::sync_with_stdio(false);
    cout << "\nRunning " << argv[0] << "\n-----\n";
    cout << "Button-chip: " << bName << ", Bounce-wait: " << bounceMsWait << "\n";
    handleArgs(argc, argv);
    cout << "Buttons-start: " << buttonsStart << ", Buttons-count: " << buttonsCount << "\n";
    cout << "Debugging mode: " << (isDebugOn ? "true" : "false") << "\n";
    cout << "-----\n\n";

    buttonChip = getChipByName(bName);
    unique_ptr<unsigned> offsets(new unsigned[buttonsCount]);
    for (int i = 0; i < buttonsCount; ++i) {
        offsets.get()[i] = buttonsStart + i;
    }
    bulkLineWrapper lines(buttonChip, offsets.get(), buttonsCount, &releaseResources);
    lines.requestFallingEdgeEvents();

    timespec timeWait = {2, 0};
    timespec timeBounce = {0, bounceMsWait * (int)1e6};
    timespec* ts = &timeWait;
    while (true) {
        auto events = lines.waitForEvent(ts);
        if (events == nullptr) {
            ts = &timeWait;
            debug("Timeout!");
            continue;
        }
        else if (ts == &timeBounce) {
            lines.readEvents(events.get());
            continue;
        }

        lines.readEvents(events.get());
        auto lineNum = gpiod_line_offset(events->lines[0]);
        debug("Get event notification on line #" + to_string(lineNum));
        ts = &timeBounce;
    }

    releaseResources();

    return 0;
}
