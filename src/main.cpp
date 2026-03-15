#include "system.hpp"

int main(void)
{
    System dkpshell("1.4.7", true);
    dkpshell.SetupMainLoop();
    dkpshell.MainLoopDkp();

    return 0;
}
