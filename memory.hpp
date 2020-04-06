#pragma once

#include <cstdlib>
#include <ctime>
#include <vector>

class memory {
    int max;
    std::vector<int> seq[2];
    int currentSeq;
    int currButton;

    void generateNext();

public:
    memory(int max);

    int count();
    void switchSeq();
    const std::vector<int>& getCurrentSeq();
    bool pushButton(int num);
};