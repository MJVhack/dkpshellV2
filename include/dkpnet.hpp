#pragma once
#include <string>

namespace DkpNet
{
    constexpr int DEFAULT_PORT = 4242;
    void Send(const std::string& ip, const std::string& filepath, int port = DEFAULT_PORT);
    void Take(int port = DEFAULT_PORT);
}