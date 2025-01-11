#include "argparser.hpp"

bool ArgParser::parseProgramArguments(int argc, char *argv[], ProgramArguments &programArgs)
{
    bool success = true;
    for (int i = 1; i < argc; ++i)
    {
        if (!processArg(argc, argv, i, programArgs))
        {
            success = false;
        }
    }
    return success;
}

bool ArgParser::processArg(int argc, char *argv[], int &idx, ProgramArguments &programArgs)
{
    if (std::strcmp(argv[idx], "--level") == 0)
    {
        ++idx;
        if (idx == argc)
        {
            return false;
        }
        int value;
        if (str2int(value, argv[idx]) != 0)
        {
            return false;
        }
        if (value < ARG_LEVEL_MIN || value > ARG_LEVEL_MAX)
        {
            return false;
        }
        programArgs.level = value;
        return true;
    }
    else if (std::strcmp(argv[idx], "-h") == 0 || std::strcmp(argv[idx], "--help") == 0)
    {
        programArgs.help = true;
        return true;
    }
    return false;
}

int ArgParser::str2int(int &res, const std::string str)
{
    {
        try
        {
            res = std::stoi(str);
        }
        catch (const std::invalid_argument &e)
        {
            return -1;
        }
        catch (const std::out_of_range &e)
        {
            return -2;
        }
        return 0;
    }
}
