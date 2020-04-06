#include <string>

#include "memory.hpp"
#include "utility.hpp"

memory::memory(int max)
{
    this->max = max;
    srand(time(0));
    currentSeq = 0;
    currButton = 0;
    generateNext();
}

int memory::count()
{
    return seq[0].size();
}

void memory::switchSeq()
{
    currButton = 0;
    if (currentSeq == 0) {
        currentSeq = 1;
    }
    else {
        currentSeq = 0;
        generateNext();
    }
    debug("Switched sequence");
}

void memory::generateNext()
{
    int num0 = rand() % max;
    debug("Pushed " + std::to_string(num0));
    seq[0].push_back(num0);

    int num1 = rand() % max;
    debug("Pushed " + std::to_string(num1));
    seq[1].push_back(num1);
}

const std::vector<int>& memory::getCurrentSeq()
{
    return seq[currentSeq];
}

bool memory::pushButton(int num)
{
    auto currSeq = getCurrentSeq();
    if (currSeq[currButton] == num) {
        ++currButton;
        return true;
    }

    currButton = 0;
    return false;
}
