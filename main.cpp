#include <iostream>

#include <cerrno>
#include <csignal>
#include <cstring>
#include <gpiod.h>
#include <unistd.h>

#include "bulkLineWrapper.hpp"
#include "globals.hpp"
#include "memory.hpp"
#include "utility.hpp"

using namespace std;
using namespace resources;

void releaseResources()
{
    gpiod_chip_close(chip);
}

gpiod_chip* getChipByName(const char* cName)
{
    gpiod_chip* chip = gpiod_chip_open_by_name(cName);
    if (chip == nullptr) {
        ERR("Open chip failed");
    }

    return chip;
}

unique_ptr<unsigned> generateOffsets(int start, int count)
{
    auto result = new unsigned[count];
    for (int i = 0; i < count; ++i) {
        result[i] = start + i;
    }

    return unique_ptr<unsigned>(result);
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
    cout << "Button-chip: " << chipName << ", Bounce-wait: " << bounceMsWait << "\n";
    handleArgs(argc, argv);
    cout << "Buttons-start: " << buttonsStart << ", Buttons-count: " << buttonsCount << "\n";
    cout << "Debugging mode: " << (isDebugOn ? "true" : "false") << "\n";
    cout << "-----\n\n";

    chip = getChipByName(chipName);

    auto buttOffsets = generateOffsets(buttonsStart, buttonsCount);
    bulkLineWrapper buttons(chip, buttOffsets.get(), buttonsCount, &releaseResources);
    debug("Prepared button lines");

    auto diodOffsets = generateOffsets(diodesStart, diodesCount);
    bulkLineWrapper diodes(chip, diodOffsets.get(), diodesCount, &releaseResources);
    debug("Prepared diode lines");

    auto memo = unique_ptr<memory>(new memory(buttonsCount));
    int pushCount = 0;
    bool diodesHighlighted = false;
    timespec timeWait = {3, 0};
    timespec timeBounce = {0, bounceMsWait * (int)1e6};
    timespec* currentWait = &timeWait;

    sleep(3);

    buttons.requestFallingEdgeEvents();
    diodes.requestOutput();
    while (true) {
        if (!diodesHighlighted) {
            auto currSeq = memo->getCurrentSeq();
            for (int i = 0; i < memo->count(); ++i) {
                diodes.setValue(currSeq[i], 1);
                usleep(12 * (int)1e5);
                diodes.setValue(currSeq[i], 0);
                usleep(8 * (int)1e5);
            }
            pushCount = 0;
            diodesHighlighted = true;
            continue;
        }
        else if (pushCount == memo->count()) {
            memo->switchSeq();
            diodesHighlighted = false;
            sleep(1);
            continue;
        }

        // Handle push
        auto events = buttons.waitForEvent(currentWait);
        if (events == nullptr) {
            currentWait = &timeWait;
            continue;
        }
        else if (currentWait == &timeBounce && pushCount != 0) {
            buttons.readEvents(events.get());
            continue;
        }

        buttons.readEvents(events.get());
        auto buttNum = gpiod_line_offset(events->lines[0]) - buttonsStart;
        cout << "Get event notification on button #" << buttNum << "\n";
        if (memo->pushButton(buttNum)) {
            ++pushCount;
        }
        else {
            cout << "You lost. Final score: " << memo->count() << "\n";
            break;
        }
        currentWait = &timeBounce;
    }

    releaseResources();
    cout << "Don't worry. Everyone loses. In the end, all men die.\n"
         << "Failure is preferable to winning through unjust means. \n"
            "How you lived is far more important than what you accomplished. \n";

    return 0;
}
