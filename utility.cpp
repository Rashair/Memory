#include <iostream>

#include "globals.hpp"
#include "utility.hpp"

void err(std::string message)
{
    std::cerr << message << std::endl;
}

void usage(std::string progName)
{
    err("USAGE:" + progName + " [-d -b buttonsStart -c buttonsCount -i diodesStart -t bounceTime (ms) -x diodesBlink (ms)]");
}

void debug(std::string message)
{
    if (resources::isDebugOn) {
        std::cout << message << "\n";
    }
}