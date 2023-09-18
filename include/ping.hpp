#ifndef COMMON
    #include "_common.hpp"
    #include "socket/socket.hpp"
#endif


int ping(Client c){
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    c.send("!ping");
    c.recv();
    while(c.getBufStr() != "!pong"){
        c.recv();
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
}