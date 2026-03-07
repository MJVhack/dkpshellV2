#include "dpd/system.hpp"

int main(void)
{
    System dkpshell("1.3.0", true);
    dkpshell.SetupMainLoop();
    dkpshell.MainLoopDkp();

    return 0;
}