#include "system.hpp"
#include <cstring>

int main(int argc, char *argv[])
{
    System dkpshell("2.1.0", true);

    char* name  = nullptr;
    char  YoN   = 'n';
    bool  start = false;

    for (int i = 1; i < argc; ++i)
    {
        if (std::strcmp(argv[i], "--name") == 0 && i + 1 < argc)
            name = argv[++i];
        else if (std::strcmp(argv[i], "--addtopath") == 0)
            YoN = 'y';
        else if (std::strcmp(argv[i], "--start") == 0)
            start = true;
    }

    dkpshell.SetupMainLoop(name, YoN, start);

    return 0;
}