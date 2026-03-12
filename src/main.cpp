#include "system.hpp"

int main(void)
{
    System dkpshell("1.4.2", true);
    dkpshell.SetupMainLoop();
    dkpshell.MainLoopDkp();

    return 0;
}
