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
   }
   
   std::cout << Color::GREEN << "Copy réussi: " << destPath << Color::RESET << std::endl;
   return true; 

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

    using FilePtr = std::unique_ptr<FILE, int(*)(FILE*)>;
    FilePtr pipe(popen(cmd.c_str(), "r"), pclose);

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

   while (1)
   {
      std::string ActualPath =  std::filesystem::current_path();
      ActualPath = CutPath(ActualPath);

      std::cout << Color::YELLOW << "[ " << Color::RED << GetHours() << Color:: YELLOW << " ] [ "<< Color::RED << InputName << "@" << Fstem << ":" << ActualPath << Color::YELLOW << " ]" << Color::RESET << std::endl;
      char* inpt = readline("$ ");
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
      }
      else if (InputCmd[0] == "dkptool")
      {
         if (InputCmd.size() > 2 && InputCmd[1] == "AddToPath")
         {
            AddToPath(InputCmd[2]);
         }
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
      
      
      
      std::cout << Color::RED << "Commande DKP non reconnu" << Color::RESET << std::endl;
      std::cout << "" << std::endl;
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

