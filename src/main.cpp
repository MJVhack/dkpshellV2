#include "system.hpp"

int main(void)
{
    System dkpshell("2.0.0", true);
    dkpshell.SetupMainLoop();
    dkpshell.MainLoopDkp();

    return 0;
}
