#include "system.hpp"

int main(void)
{
    System dkpshell("1.3.3", true);
    dkpshell.SetupMainLoop();
    dkpshell.MainLoopDkp();

    return 0;
}
