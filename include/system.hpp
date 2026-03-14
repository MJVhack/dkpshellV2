#pragma once 
#include <string>
#include <vector>
#include <memory>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <unistd.h>
#include <limits.h>
#include <cstdio>
#include <sstream>
#include <ctime>
#include <readline/readline.h>
#include <readline/history.h>
#include <cstring>
#include <pwd.h>
#include <algorithm>
#include <array>
#include <sys/wait.h>

namespace Color 
{
    const std::string RED          = "\033[31m";
    const std::string GREEN        = "\033[32m";
    const std::string YELLOW       = "\033[33m";
    const std::string BLUE         = "\033[34m";
    const std::string MAGENTA      = "\033[35m";
    const std::string CYAN         = "\033[36m";
    const std::string WHITE        = "\033[37m";
    const std::string RESET        = "\033[0m";

    const std::string BOLD         = "\033[1m";
    const std::string DIM          = "\033[2m";
    const std::string UNDERLINE    = "\033[4m";
    const std::string BLINK        = "\033[5m";

    const std::string BG_RED       = "\033[41m";
    const std::string BG_GREEN     = "\033[42m";
    const std::string BG_YELLOW    = "\033[43m";
    const std::string BG_BLUE      = "\033[44m";
    const std::string BG_MAGENTA   = "\033[45m";
    const std::string BG_CYAN      = "\033[46m";
    const std::string BG_WHITE     = "\033[47m";
}



class System
{
private:

    const std::string asciiart = R"(
<!-- .------------------------------------------------------------------------------------------------. -->
<!-- |                                                                                                | -->
<!-- |                                                                                                | -->
<!-- |                                                                                                | -->
<!-- |                                                                                                | -->
<!-- |                                                                                                | -->
<!-- |     ________    ____  __.__________    _________  ___ ___  ___________.____     .____          | -->
<!-- |     \______ \  |    |/ _|\______   \  /   _____/ /   |   \ \_   _____/|    |    |    |         | -->
<!-- |      |    |  \ |      <   |     ___/  \_____  \ /    ~    \ |    __)_ |    |    |    |         | -->
<!-- |      |    `   \|    |  \  |    |      /        \\    Y    / |        \|    |___ |    |___      | -->
<!-- |     /_______  /|____|__ \ |____|     /_______  / \___|_  / /_______  /|_______ \|_______ \     | -->
<!-- |             \/         \/                    \/        \/          \/         \/        \/     | -->
<!-- |                                                                                                | -->
<!-- |                                                                                                | -->
<!-- |                                                                                                | -->
<!-- |                                                                                                | -->
<!-- |                                                                                                | -->
<!-- '------------------------------------------------------------------------------------------------' -->
                                           BY DKP
    )";
    const bool __stable__;
    const std::string __version__;

    std::vector<std::string> helplist = {"HELP LIST", 
        "Toute les commandes exécuté ici se comporterons exactement de la même faço que d'en votre shell normal",
        "Toute les customs command: ",
        "dkpconfig",
        "dkptool",
        "dkptheme"};
    std::vector<std::string> HLdkpconfig = {"DKPCONFIG help list",
    "dkpconfig -restartshell : Redémarre le script",
    "dkptool AddToPath {path}",
    "dkptheme P1/2/3/4/5"};

    std::vector<std::string> UpdList = {"Ajout de la commande: dkptool AddToPath", "1.3.0: auto completion", "1.3.0+ thème, history etc", "1.4.0+ theme, optimize etc"};
    std::vector<std::string> CmdList = {"dkpconfig", "dkptool", "exit"};
    std::vector<std::string> InputCmd;
    std::string InputName;

    const std::string URL_V = "https://raw.githubusercontent.com/MJVhack/dkpshellV2/refs/heads/master/version.txt";
    const std::string URL_N = "https://raw.githubusercontent.com/MJVhack/dkpshellV2/master/compile/dkpshell.out";



    const char* homedir = std::getenv("HOME");

    
    

public:
    System();
    System(System& Isystem);
    System(std::string version, bool stable);

    ~System();

    std::string GetVersion();
    bool GetStability();

    void RestartShell();

    void DisplayHelplist(std::string Thp);

    std::string SetupMainLoop();

    bool AddToPath(std::filesystem::path Fpath);

    std::filesystem::path GetFile();
    std::pair<int, std::string> runCommand(const std::string& cmd);

    void MainLoopDkp();

    std::vector<std::string> GetArg(std::string line);

    std::string GetHours();

    std::string CutPath(std::string path);

    void UpdateScr();

    std::string GetGitBranch();

    void ClearOSS(std::ostringstream& OSS);

    void dkptheme(int& CT, const std::vector<std::string>& INPCMD);
};
