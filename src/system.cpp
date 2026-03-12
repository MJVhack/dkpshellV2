#include "system.hpp"

 System::System()
    : __version__("0.0.1")
    , __stable__(false)
 {
    InputName.reserve(200);
 }

 System::System(System& system)
    : __version__(system.__version__)
    , __stable__(system.__stable__)
 {
    InputName.reserve(200);
 }

 System::System(std::string version, bool stable)
    : __version__(version)
    , __stable__(stable)
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


void System::DisplayHelplist(std::string Thl)
{
   if (Thl == "hp")
   {
      for (int i = 0; i < helplist.size(); ++i)
      {
         std::cout << Color::BLUE << helplist[i] << Color::RESET << std::endl;
      }
   }
   else if (Thl == "dkphp")
   {
      for (int i = 0; i < HLdkpconfig.size(); ++i)
      {
         std::cout << Color::BLUE << HLdkpconfig[i] << Color::RESET << std::endl;
      }
   }
   else if (Thl == "updlist")
   {
      for (int i = 0; i < UpdList.size(); ++i)
      {
         std::cout << Color::BLUE << UpdList[i] << Color::RESET << std::endl;
      }
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
   std::stringstream cline(line);
   std::vector<std::string> MultiW;
   std::string OneW;

   while (cline >> OneW)
   {
      MultiW.push_back(OneW);
   }
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

std::string System::GetGitBranch()

{
   std::string cmd = "git -C . symbolic-ref --short HEAD 2>/dev/null";
   std::string branch = runCommand(cmd);

   if (!branch.empty() && branch.back() == '\n')
      branch.pop_back();

   return branch;
}

void System::UpdateScr()
{
   // Tsais quoi ? nike libcurl.h
   std::string Cmd = "wget -qO- " + URL_V; 
   std::string VDown = runCommand(Cmd);

   VDown.erase(std::remove(VDown.begin(), VDown.end(), '\n'), VDown.end());
   VDown.erase(std::remove(VDown.begin(), VDown.end(), '\r'), VDown.end());

   if (VDown == __version__)
   {
      std::cout << Color::GREEN << "Aucune MAJ a faire." << Color::RESET << std::endl;
   }
   else
   {
      std::string rep;
      std::cout << Color::BLUE << "Nouvelle version disponible (" << VDown << "), voulez vous l'installer [y/n]: " << Color::RESET;
      std::getline(std::cin, rep);

      if (rep == "y" || rep == "Y")
      {
         Cmd = "wget -O /tmp/dkpshell.out " + URL_N; 
         runCommand(Cmd);

         Cmd = "rm /usr/local/bin/dkpshell";
         runCommand(Cmd);

         Cmd = "mv /tmp/dkpshell.out /usr/local/bin/dkpshell";
         runCommand(Cmd);

         Cmd = "chmod +x /usr/local/bin/dkpshell";
         runCommand(Cmd);

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
   DisplayHelplist("updlist");

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

std::string System::runCommand(const std::string& cmd)
{
    std::string output;
    std::string finalCmd = "bash -c " + std::string(1, '"') + cmd + std::string(1, '"');

    using FilePtr = std::unique_ptr<FILE, int(*)(FILE*)>;
    FilePtr pipe(popen(finalCmd.c_str(), "r"), pclose);

    if (!pipe)
        return "Erreur : impossible d'ouvrir le pipe\n";

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr)
        output += buffer;

    return output;
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
    if (hour == 0) hour = 12; // minuit

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


void System::MainLoopDkp()
{
   std::filesystem::path fname = GetFile();
   std::string Fstem = fname.stem();
   Fstem = CutPath(Fstem);
   read_history("~/.dkpshell-history");
   std::ostringstream P1;
   std::string P1s;
   std::ostringstream P2;
   std::string P2s;
   std::ostringstream P3;
   std::string P3s;


   std::ostringstream ActualTheme;
   std::string ActualThemeSuite;

   int curentTheme = 1;

  

   while (1)
   {
      struct passwd* pw = getpwuid(getuid());
      std::string username;
      if (pw)
         username = pw->pw_name;
      else
         username = (getuid() == 0) ? "root" : "user";


      std::string ActualPath =  std::filesystem::current_path();
      ActualPath = CutPath(ActualPath);

      P1.str("");
      P1.clear();
      P1 << Color::YELLOW << "[ " 
      << Color::RED << GetHours() 
      << Color:: YELLOW << " ] [ "
      << Color::RED << InputName 
      << "@" << Fstem 
      << ":" << ActualPath 
      << Color::YELLOW << " ] (" 
      << Color::RED << GetGitBranch() 
      << Color::YELLOW << ")"
      << Color::RESET;
      P1s = "$ ";

      P2.str("");
      P2.clear();
      P2 << Color::RED << "┌─[" 
         << Color::CYAN << InputName 
         << "@" << Fstem 
         << Color::RED << "] [" 
         << Color::CYAN << ((getuid() == 0) ? "root":"user") 
         << Color::RED << "] [" 
         << Color::CYAN << GetGitBranch()
         << Color::RED << "]";

      P2s = "└─[" + Color::CYAN + ActualPath + Color::RED + "]> " + Color::RESET;

      P3.str("");
      P3.clear();
      P3 << Color::BLUE << "[ "
         << Color::MAGENTA << username
         << " | " << ActualPath
         << Color::BLUE << " ] - ("
         << Color::MAGENTA << GetGitBranch()
         << Color::BLUE << ")";

      P3s = Color::BLUE + ">> " + Color::RESET;




      if (curentTheme == 1)
      {
         ActualTheme.str(P1.str());
         ActualThemeSuite = P1s;
      }
      else if (curentTheme == 2)
      {
         ActualTheme.str(P2.str());
         ActualThemeSuite = P2s;
      }
      else if (curentTheme == 3)
      {
         ActualTheme.str(P3.str());
         ActualThemeSuite = P3s;
      }
      
      

      


      //std::cout << Color::YELLOW << "[ " << Color::RED << GetHours() << Color:: YELLOW << " ] [ "<< Color::RED << InputName << "@" << Fstem << ":" << ActualPath << Color::YELLOW << " ]" << Color::RESET << std::endl;
      std::cout << ActualTheme.str() << std::endl;
      char* inpt = readline(ActualThemeSuite.c_str());
      if (inpt == nullptr) {write_history("~/.dkpshell-history"); exit(0);}
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
         DisplayHelplist("dkphp");
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
         write_history("~/.dkpshell-history");
         exit(0);
      }
      else if (InputCmd[0] == "dkptheme")
      {
         if (InputCmd[1] == "P1")
         {
            curentTheme = 1;
         }
         else if (InputCmd[1] == "P2")
         {
            curentTheme = 2;
         }
         else if (InputCmd[1] == "P3")
         {
            curentTheme = 3;
         }
         

         continue;
         
      }
      
      
      
      
      std::string result = runCommand(line);

      if (result.find("Erreur") != std::string::npos)
      {
         std::cout << Color::RED << result << Color::RESET << std::endl;
      }
      else
      {
         std::cout << Color::GREEN << result << Color::RESET << std::endl;
      }
      
   }
}

