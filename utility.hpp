#ifndef UTILITY_H
#define UTILITY_H

#include "signal.h"
#include <string>

#if !defined(ERR)
#define ERR(source) (fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), perror(source), kill(0, SIGKILL), exit(EXIT_FAILURE))
#endif // ERR

void err(std::string message);
void usage(std::string progName);
void debug(std::string message);

#endif