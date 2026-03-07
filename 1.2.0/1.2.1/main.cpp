#include "dpd/system.hpp"

int main(void)
{
    System dkpshell("1.2.1", true);
    dkpshell.SetupMainLoop();
    dkpshell.MainLoopDkp();

    return 0;
}