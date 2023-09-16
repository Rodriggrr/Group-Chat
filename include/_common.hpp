#define COMMON

#define LOCALHOST "127.0.0.1"
#define LOCALHOST0 "0.0.0.0"

#ifdef __linux__
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <unistd.h>
    #include <sys/socket.h>
#endif

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <windows.h>
    #include <Winerror.h>
#endif

#include <memory>
#include <cstdio>
#include <ctime>
#include <cstring>
#include <iostream>
#include <thread>
#include <vector>
#include "fn.hpp"

void printr(std::string msg){
    std::cout << '\r' << msg << std::endl;
}

int mutex = 0;

void lock(){
    while(mutex);
    mutex = 1;
}

void unlock(){
    mutex = 0;
}

std::string horaAtual() {
    auto agora = std::chrono::system_clock::now();
    auto tempo = std::chrono::system_clock::to_time_t(agora);
    std::tm tempoLocal = *std::localtime(&tempo);

    std::ostringstream oss;
    oss << "[" << std::put_time(&tempoLocal, "%H:%M") << "]";
    
    return oss.str();
}
