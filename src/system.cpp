#include "system.hpp"

 System::System()
    : __version__("0.0.1")
    , __stable__(false)
    , P(10)
    , Ps(10)
 {
    InputName.reserve(200);
 }

 System::System(System& system)
    : __version__(system.__version__)
    , __stable__(system.__stable__)
    , P(10)
    , Ps(10)
 {
    InputName.reserve(200);
 }

 System::System(std::string version, bool stable)
    : __version__(version)
    , __stable__(stable)
    , P(10)
    , Ps(10)
 {
    InputName.reserve(200);
 }

 System::~System()

 {}

std::string System::GetVersion()
 {
    return __version__;
 }

bool System::GetStability()
 {
    return __stable__;
 }


void System::DisplayHelplist(std::vector<std::string>& hlp)
{
   for (int i = 0; i < hlp.size(); ++i)
   {
      std::cout << Color::BLUE << hlp[i] << Color::RESET << std::endl;
   }
}

void System::RestartShell()
{
   std::filesystem::path SelfPath = GetFile();
   std::string pathStr = SelfPath.string();  
   char* pathC = const_cast<char*>(pathStr.c_str());  
   char* args[] = { pathC, nullptr };  
   if (execv(pathC, args) == -1)
   {
      perror("execv");
   }

}

std::vector<std::string> System::GetArg(std::string line)
{
    std::vector<std::string> MultiW;
    std::string token;
    bool inSingle = false;
    bool inDouble = false;

    for (size_t i = 0; i < line.size(); ++i)
    {
        char c = line[i];

        if (c == '\'' && !inDouble)
        {
            inSingle = !inSingle;   
        }
        else if (c == '"' && !inSingle)
        {
            inDouble = !inDouble;   
        }
        else if (c == ' ' && !inSingle && !inDouble)
        {
            if (!token.empty())     
            {
                MultiW.push_back(token);
                token.clear();
            }
        }
        else
        {
            token += c;
        }
    }

    if (!token.empty())
        MultiW.push_back(token);   

    return MultiW;
}

bool System::AddToPath(std::filesystem::path Fpath)
{
   std::filesystem::path dest("/usr/local/bin");
   std::filesystem::path destPath = dest / Fpath.stem();

   try
   {
      std::filesystem::copy_file(Fpath, destPath,
                      std::filesystem::copy_options::overwrite_existing);
   }
   catch(const std::filesystem::filesystem_error& e)
   {
      std::cerr << Color::RED << e.what() << Color::RESET << std::endl;
      return false;
   }
   
   std::cout << Color::GREEN << "Copy réussi: " << destPath << Color::RESET << std::endl;
   return true; 

}

void System::AddAlias(const std::string& name, const std::string& cmd) {
    std::ofstream file(aliasP, std::ios::app);
    if (file.is_open()) {                      
        file << name << "=" << cmd << "\n";   
    }
}

void System::RemoveAlias(const std::string& name) {
    std::ifstream file(aliasP);                     
    std::map<std::string, std::string> aliases;  
    std::string line;

    while (std::getline(file, line)) {           
        size_t pos = line.find('=');             
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos); 
            if (key != name) {                    
                aliases[key] = line.substr(pos + 1); 
            }
        }
    }
    file.close();

    std::ofstream out(aliasP, std::ios::trunc);    
    for (auto& [key, val] : aliases) {
        out << key << "=" << val << "\n";        
    }
}

void System::ListAlias()
{
   std::string cmdcat = "cat /.dkpalias";
   auto[code, alias] = runCommand(cmdcat);
   if (code != 0) std::cout << Color::RED << alias << Color::RESET << std::endl;
   else std::cout << Color::GREEN << alias << Color::RESET << std::endl;
   
}

void System::ModifyAlias(const std::string& name, const std::string& newCmd) {
    std::ifstream file(aliasP);
    std::map<std::string, std::string> aliases;
    std::string line;

    while (std::getline(file, line)) {
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string val = line.substr(pos + 1);
            if (key == name) val = newCmd;
            aliases[key] = val;
        }
    }
    file.close();

    std::ofstream out(aliasP, std::ios::trunc);    
    for (auto& [key, val] : aliases) {
        out << key << "=" << val << "\n";
    }
}

void System::ScriptInfo()
{
   std::cout << Color::CYAN << "Version: "
   << __version__ << std::endl << "Stability: "
   << __stable__ << std::endl << "Github script: "
   << GitHubLink << std::endl << "Creator: "
   << Creator << std::endl << "GitHub: " 
   << GitHub << std::endl << "Discord: "
   << Discord << Color::RESET << std::endl;   
}

std::string System::GetGitBranch()

{
   std::string cmd = "git -C . symbolic-ref --short HEAD 2>/dev/null";
   auto [code, branch] = runCommand(cmd);

   if (!branch.empty() && branch.back() == '\n')
      branch.pop_back();

   return branch;
}

void System::UpdateScr()
{
   int code;
   std::string out;
   // Tsais quoi ? nike libcurl.h
   std::string Cmd = "wget -qO- " + URL_V; 
   std::tie(code, out) = runCommand(Cmd);
   if (code != 0)
   {
      std::cout << Color::RED << "Error" << Color::RESET << std::endl;
      return;
   }

   out.erase(std::remove(out.begin(), out.end(), '\n'), out.end());
   out.erase(std::remove(out.begin(), out.end(), '\r'), out.end());

   if (out == __version__)
   {
      std::cout << Color::GREEN << "Aucune MAJ a faire." << Color::RESET << std::endl;
   }
   else
   {
      std::string rep;
      std::cout << Color::BLUE << "Nouvelle version disponible (" << out << "), voulez vous l'installer [y/n]: " << Color::RESET;
      std::getline(std::cin, rep);

      if (rep == "y" || rep == "Y")
      {
         Cmd = "wget -O /tmp/dkpshell.out " + URL_N; 
         std::tie(code, out) = runCommand(Cmd);
         if (code != 0)
         {
            std::cout << Color::RED << "Error" << Color::RESET << std::endl;
            return;
         }

         Cmd = "rm /usr/local/bin/dkpshell";
         std::tie(code, out) = runCommand(Cmd);
         if (code != 0)
         {
            std::cout << Color::RED << "Error" << Color::RESET << std::endl;
            return;
         }

         Cmd = "mv /tmp/dkpshell.out /usr/local/bin/dkpshell";
         std::tie(code, out) = runCommand(Cmd);
         if (code != 0)
         {
            std::cout << Color::RED << "Error" << Color::RESET << std::endl;
            return;
         }


         Cmd = "chmod +x /usr/local/bin/dkpshell";
         std::tie(code, out) = runCommand(Cmd);
         if (code != 0)
         {
            std::cout << Color::RED << "Error" << Color::RESET << std::endl;
            return;
         }

         std::cout << Color::GREEN << "Execution au prochain dkpshell" << Color::RESET << std::endl;
      }
   }
}

std::string System::SetupMainLoop()
{
   // On clear
   system("clear");

   //Ascii art bien géant
   std::cout << Color::MAGENTA << asciiart << Color::RESET << std::endl;
   std::cout << "__version__: " << __version__ << std::endl;
   std::cout << "__stable__: " << __stable__ << std::endl;
   DisplayHelplist(UpdList);

   // Prefix 
    std::cout << Color::GREEN << "Voici le patterne: [ TIME ] [ prefix@scritname:actual_path ] $" << Color::RESET << std::endl;
    std::cout << Color::BLUE << "Entrez le préfixe de votre terminal: " << Color::RESET ;
    std::getline(std::cin, InputName);

   //AddToPath
    if (getuid() == 0) 
    {
      char YoN;
      std::cout << Color::YELLOW << "Privilège administrateur détecté" << Color::RESET << std::endl;
      std::cout << Color::BLUE << "Est-ce que tu veux l'ajouter au PATH ?[y/n]: " << Color::RESET;
      std::cin >> YoN;
      std::cin.ignore();
      switch (YoN)
      {
      case 'y': {
         bool ATP = AddToPath(GetFile());
         std::cout << ((ATP) ? Color::GREEN:Color::RED)<< "AddToPath " << ((ATP) ? "réussi.":"échoué.") << Color::RESET << std::endl;
         break;
      }

      case 'n':
         break;
      
      }
      UpdateScr();

    }
    std::string inpSt;
    std::cout << Color::RED << "Entrez 'start' pour lancer le script: " << Color::RESET;
    std::cin >>inpSt;

    if (inpSt == "start")
    {
      MainLoopDkp();
      return "started";
    }
    else
    {
      std::cout << Color::RED << "Quit" << Color::RESET << std::endl;
      exit(0);
      return "quit";
    }
    

}

std::filesystem::path System::GetFile()
{
   std::string buffer;
   buffer.resize(PATH_MAX);
   ssize_t len = readlink("/proc/self/exe", buffer.data(), buffer.size() - 1);
   if (len == -1)
   {
      std::cout << Color::RED << "Readlink error" << Color::RESET << std::endl;
      return "";
   }

   buffer[len] = '\0';
   buffer.resize(len);
   std::filesystem::path exePath(buffer);

   return exePath;
}

std::pair<int, std::string> System::runCommand(const std::string& cmd)
{
    std::string output;
    std::string finalCmd = "bash -c \"" + cmd + "\"";

    using FilePtr = std::unique_ptr<FILE, int(*)(FILE*)>;
    FilePtr pipe(popen(finalCmd.c_str(), "r"), pclose);

    if (!pipe)
        return {-1, "Erreur : impossible d'ouvrir le pipe\n"};

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr)
        output += buffer;

    int retCode = pclose(pipe.release()); 
    if (WIFEXITED(retCode))
        retCode = WEXITSTATUS(retCode);
    else
        retCode = -1; 

    return {retCode, output};
}

void System::ClearOSS(std::ostringstream& OSS)
{
   OSS.str("");
   OSS.clear();
}

std::string System::transformVtoSS(const std::vector<std::string>& INPCMD)
{
   std::string cmd;

   for (size_t i = 0; i < INPCMD.size(); ++i)
   {
     if (i > 0)
         cmd += " ";

     cmd += INPCMD[i];
   }

   return cmd;
   
}

std::vector<std::string> System::loadOrCreateAliasFile(const std::string& path) {
    std::vector<std::string> lines;

    if (!fs::exists(path)) {
        std::ofstream createFile(path);
        if (!createFile.is_open()) {
            std::cerr << "Impossible de créer le fichier " << path << "\n";
            return lines;
        }
        createFile.close();
        std::cout << "Fichier créé : " << path << "\n";
    }

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Impossible d'ouvrir le fichier " << path << "\n";
        return lines;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) lines.push_back(line); 
    }
    file.close();

    return lines; 
}

std::string System::GetHours()
{
    std::time_t t = std::time(nullptr);
    std::tm* localTime = std::localtime(&t);

    int hour = localTime->tm_hour;
    int minute = localTime->tm_min;
    std::string am_pm = "AM";

    if (hour >= 12) {
        am_pm = "PM";
        if (hour > 12) hour -= 12;
    }
    if (hour == 0) hour = 12; 

    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << hour
        << ":"
        << std::setw(2) << std::setfill('0') << minute
        << am_pm;

    return oss.str();
}

std::string System::CutPath(std::string path)
{
   if (path.size() >= 2 && path.front() == '"' && path.back() == '"')
   {
      path = path.substr(1, path.size() - 2);
   }

   return path;
}
void System::dkptheme(int& CT, const std::vector<std::string>& INPCMD)
{
   if (INPCMD[1] == "P1") CT = 1;
   else if (INPCMD[1] == "P2") CT = 2;
   else if (INPCMD[1] == "P3") CT = 3;
   else if (INPCMD[1] == "P4") CT = 4;
   else if (INPCMD[1] == "P5") CT = 5;
   else if (INPCMD[1] == "P6") CT = 6;
   else DisplayHelplist(HLdkpconfig);
}

void System::MainLoopDkp()
{
   std::filesystem::path fname = GetFile();
   std::string Fstem = fname.stem();
   Fstem = CutPath(Fstem);
   
   int curentTheme = 1;

   // std::ostringstream P1;
   // std::string P1s;
   // std::ostringstream P2;
   // std::string P2s;
   // std::ostringstream P3;
   // std::string P3s;
   // std::ostringstream P4;
   // std::string P4s;
   // std::ostringstream P5;
   // std::string P5s;

   std::ostringstream ActualTheme;
   std::string ActualThemeSuite;
   
   while (1)
   {
      auto aliasLines = System::loadOrCreateAliasFile();
      std::map<std::string,std::string> aliases;
      for (auto& line : aliasLines) {
         size_t pos = line.find('=');
         if (pos != std::string::npos) {
            aliases[line.substr(0,pos)] = line.substr(pos+1);
         }
      }
      struct passwd* pw = getpwuid(getuid());
      std::string username;
      if (pw)
         username = pw->pw_name;
      else
         username = (getuid() == 0) ? "root" : "user";

      std::string ActualPath =  std::filesystem::current_path();
      ActualPath = CutPath(ActualPath);

      ClearOSS(P[0]);
      P[0] << Color::YELLOW << "[ " 
         << Color::RED << GetHours() 
         << Color:: YELLOW << " ] [ "
         << Color::RED << InputName 
         << "@" << Fstem 
         << ":" << ActualPath 
         << Color::YELLOW << " ] (" 
         << Color::RED << GetGitBranch() 
         << Color::YELLOW << ")"
         << Color::RESET;
      Ps[0] = "$ ";

      ClearOSS(P[1]);
      P[1] << Color::RED << "┌─[" 
         << Color::CYAN << InputName 
         << "@" << Fstem 
         << Color::RED << "] [" 
         << Color::CYAN << ((getuid() == 0) ? "root":"user") 
         << Color::RED << "] [" 
         << Color::CYAN << GetGitBranch()
         << Color::RED << "]";

      Ps[1] = "└─[" + Color::CYAN + ActualPath + Color::RED + "]> " + Color::RESET;

      ClearOSS(P[2]);
      P[2] << Color::BLUE << "[ "
         << Color::MAGENTA << username
         << " | " << ActualPath
         << Color::BLUE << " ] - ("
         << Color::MAGENTA << GetGitBranch()
         << Color::BLUE << ")";

      Ps[2] = Color::BLUE + ">> " + Color::RESET;

      ClearOSS(P[3]);
      P[3] << Color::GREEN << "{ "
         << Color::BLUE << Color::BOLD
         << "PATH:" << ActualPath
         << Color::RESET << Color::BLUE 
         << " : " << Color::BOLD 
         << username << Color::RESET
         << Color::GREEN << " ["
         << Color::BLUE << Color::BOLD
         << GetGitBranch() << Color::RESET 
         << Color::GREEN << "] }" 
         << Color::RESET;
      
      Ps[3] = "; ";
      
      ClearOSS(P[4]);
      P[4] << Color::YELLOW << username
         << Color::MAGENTA << ": "
         << Color::YELLOW << ((getuid() == 0) ? "root" : "user")
         << Color::MAGENTA << " / "
         << Color::YELLOW << GetGitBranch()
         << "                                                                                                 " << GetHours()
         << Color::RESET;

      Ps[4] =  Color::RED + ActualPath + "> " + Color::RESET;
      
      ClearOSS(P[5]);
      P[5].str("NULL");

      Ps[5] = "% ";

      if (curentTheme == 1)
      {
         ActualTheme.str(P[0].str());
         ActualThemeSuite = Ps[0];
      }
      else if (curentTheme == 2)
      {
         ActualTheme.str(P[1].str());
         ActualThemeSuite = Ps[1];
      }
      else if (curentTheme == 3)
      {
         ActualTheme.str(P[2].str());
         ActualThemeSuite = Ps[2];
      }
      else if (curentTheme == 4)
      {
         ActualTheme.str(P[3].str());
         ActualThemeSuite = Ps[3];
      }
      else if (curentTheme == 5)
      {
         ActualTheme.str(P[4].str());
         ActualThemeSuite = Ps[4];
      }
      else if (curentTheme == 6)
      {
         ActualTheme.str(P[5].str());
         ActualThemeSuite = Ps[5];
      }
      

      if (ActualTheme.str() != "NULL") std::cout << ActualTheme.str() << std::endl;
      char* inpt = readline(ActualThemeSuite.c_str());
      if (inpt == nullptr) exit(0);
      std::string line(inpt);
      free(inpt);
      if (!line.empty()) add_history(line.c_str());
      if (line.empty()) continue;
      InputCmd = GetArg(line);

      if (InputCmd[0] == "dkpconfig")
      {
         if (InputCmd.size() > 1 && InputCmd[1] == "-restartshell")
         {
            RestartShell();
            continue;
         }
         DisplayHelplist(HLdkpconfig);
         continue;
      }
      else if (InputCmd[0] == "dkptool")
      {
         if (InputCmd.size() > 2 && InputCmd[1] == "AddToPath")
         {
            AddToPath(InputCmd[2]);
         }
         continue;
      }
      else if (InputCmd[0] == "cd")
      {
         chdir(InputCmd[1].c_str());
         continue;
      }
      else if (InputCmd[0] == "exit")
      {
         std::cout << Color::RED << "Exit" << Color::RESET << std::endl;
         exit(0);
      }
      else if (InputCmd[0] == "dkptheme")
      {
         dkptheme(curentTheme, InputCmd);
         continue;
         
      }
      else if (InputCmd[0] == "apt")
      {
         std::cout << Color::YELLOW << "Apt doesn't have a good CLI interface, trasform apt to apt-get..." << std::endl;
         InputCmd[0] = "apt-get";
         std::string cmd = transformVtoSS(InputCmd);
         auto[code, out] = runCommand(cmd);

         if (code != 0) std::cout << Color::RED << out << Color::RESET << std::endl;
         else std::cout << Color::GREEN << out << Color::RESET << std::endl;

      }
      else if (InputCmd[0] == "dkpalias")
      {
         if (InputCmd.size() < 2) { DisplayHelplist(AliasHL); continue; }

         auto buildCmd = [&](size_t from) {
            std::string val;
            for (size_t i = from; i < InputCmd.size(); ++i) {
               if (i > from) val += " ";
               if (InputCmd[i].find(' ') != std::string::npos)
                  val += "\"" + InputCmd[i] + "\"";
               else
                  val += InputCmd[i];
            }
            return val;
         };

         if (InputCmd[1] == "set") {
            if (InputCmd.size() < 4) { DisplayHelplist(AliasHL); continue; }
            AddAlias(InputCmd[2], buildCmd(3));
         }
         else if (InputCmd[1] == "remove") {
            if (InputCmd.size() < 3) { DisplayHelplist(AliasHL); continue; }
            RemoveAlias(InputCmd[2]);
         }
         else if (InputCmd[1] == "modify") {
            if (InputCmd.size() < 4) { DisplayHelplist(AliasHL); continue; }
            ModifyAlias(InputCmd[2], buildCmd(3));
         }
         else if (InputCmd[1] == "listalias") {
            ListAlias();
         }
         else if (InputCmd.size() >= 3) {
            AddAlias(InputCmd[1], buildCmd(2));
         }
         else {
         DisplayHelplist(AliasHL);
         }
         continue;
      }
      else if (InputCmd[0] == "dkpinfo")
      {
         ScriptInfo();
         continue;
      }
      else if (InputCmd[0] == "dkp")
      {
         DisplayHelplist(CmdList);
         continue;
      }
      
      
      
      if (!InputCmd.empty() && aliases.find(InputCmd[0]) != aliases.end()) {
         std::string aliasCmd = aliases[InputCmd[0]];


         for (size_t i = 1; i < InputCmd.size(); i++) {
            aliasCmd += " " + InputCmd[i];
         }

         auto [code, out] = runCommand(aliasCmd);
         if (code != 0) std::cout << Color::RED << out << Color::RESET << std::endl;
         else std::cout << Color::GREEN << out << Color::RESET << std::endl;
         continue;
      }
      
      
      
      auto [code, out] = runCommand(line);

      if (code != 0) std::cout << Color::RED << out << Color::RESET << std::endl;
      else std::cout << Color::GREEN << out << Color::RESET << std::endl;
      
   }
}

