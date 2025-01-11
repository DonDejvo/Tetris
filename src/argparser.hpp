#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <iostream>
#include <string>
#include <cstring>

#define ARG_LEVEL_DEFAULT 0
#define ARG_LEVEL_MIN 0
#define ARG_LEVEL_MAX 9

struct ProgramArguments
{
    int level = ARG_LEVEL_DEFAULT;
    bool help = false;
};

class ArgParser
{
public:
    static bool parseProgramArguments(int argc, char *argv[], ProgramArguments &programArgs);

private:
    static bool processArg(int argc, char *argv[], int &idx, ProgramArguments &programArgs);
    static int str2int(int &res, const std::string str);
};

#endif