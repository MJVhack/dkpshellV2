#include "dkpnet.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sys/socket.h>   
#include <netinet/in.h>  
#include <arpa/inet.h>    
#include <unistd.h>       

namespace DkpNet
{

void Take(int port)
{
    int serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd < 0)
    {
        std::cerr << "Erreur : création du socket impossible\n";
        return;
    }
    int opt = 1;
    setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    sockaddr_in addr{};
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port        = htons(port); 

    if (bind(serverFd, (sockaddr*)&addr, sizeof(addr)) < 0)
    {
        std::cerr << "Erreur bind (port déjà utilisé ?)\n";
        close(serverFd);
        return;
    }

    listen(serverFd, 1);
    std::cout << "📡 En attente sur le port " << port << "...\n";

    sockaddr_in clientAddr{};
    socklen_t   clientLen = sizeof(clientAddr);
    int clientFd = accept(serverFd, (sockaddr*)&clientAddr, &clientLen);
    if (clientFd < 0)
    {
        std::cerr << "Erreur accept\n";
        close(serverFd);
        return;
    }

    std::string filename;
    char c;
    while (read(clientFd, &c, 1) > 0 && c != '\n')
        filename += c;

    if (filename.empty())
    {
        std::cerr << "Nom de fichier vide, abandon.\n";
        close(clientFd);
        close(serverFd);
        return;
    }

    std::cout << "📥 Réception de : " << filename << "\n";
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile)
    {
        std::cerr << "Impossible de créer le fichier : " << filename << "\n";
        close(clientFd);
        close(serverFd);
        return;
    }

    char   buffer[4096]; 
    ssize_t bytesRead;

    while ((bytesRead = read(clientFd, buffer, sizeof(buffer))) > 0)
        outFile.write(buffer, bytesRead);

    std::cout << "✅ Fichier reçu.\n";

    close(clientFd);
    close(serverFd);
}

void Send(const std::string& ip, const std::string& filepath, int port)
{
    std::ifstream inFile(filepath, std::ios::binary);
    if (!inFile)
    {
        std::cerr << "Fichier introuvable : " << filepath << "\n";
        return;
    }

    int sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFd < 0)
    {
        std::cerr << "Erreur : création du socket impossible\n";
        return;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port   = htons(port);

    if (inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr) <= 0)
    {
        std::cerr << "IP invalide : " << ip << "\n";
        close(sockFd);
        return;
    }


    if (connect(sockFd, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cerr << "Connexion échouée. L'autre machine a lancé 'dkpnet take' ?\n";
        close(sockFd);
        return;
    }

    std::string filename = filepath.substr(filepath.find_last_of("/\\") + 1);
    std::string header   = filename + "\n";
    send(sockFd, header.c_str(), header.size(), 0);

    char buffer[4096];
    while (inFile.read(buffer, sizeof(buffer)) || inFile.gcount() > 0)
        send(sockFd, buffer, (size_t)inFile.gcount(), 0);

    std::cout << "✅ Fichier envoyé.\n";

    close(sockFd);
}

} 