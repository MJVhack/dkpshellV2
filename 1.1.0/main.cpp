#include "dpd/system.hpp"

int main(void)
{
    System dkpshell("1.1.0", true);
    dkpshell.SetupMainLoop();
    dkpshell.MainLoopDkp();

    return 0;
}