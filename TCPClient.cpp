#ifndef COMMON
    #include "include/_common.hpp"
#endif
#include "include/socket/socket.hpp"

Socket client(LOCALHOST, 49100);
bool connected = false;
bool whisper = false;
std::string host = "";

void inputHandler(){
    std::string msg;
    getline(std::cin, msg);
    if (msg.size() == 0) msg = " ";
    client.send(msg);


    while(true){
        while(host == "");
            std::cout << host << (whisper ? "(sussurro): " : ": ");

        getline(std::cin, msg);
        if (msg.size() == 0) msg = " ";

        if(host != "" and msg[0] != '!'){    
            printr("\033[F" + horaAtual() + host + ((whisper) ? "(sussurro): " : ": ") + msg + "          ");
        }
        client.send(msg);
    }
}

void incomingHandler(){
    while(true){
        int bytes = client.recv();
        if(bytes < 1) break;

        auto args = fn::split(client.getBufStr(), ' ');
        if(args[0] == "!disconnect") break;
        if(args[0] == "!host") {
            lock();
            args.erase(args.begin());
            host = fn::join(args, " ");
            unlock();
            continue;
        }
        if(args[0] == "!whisper") {
            ::whisper = true;
        }
        else if(args[0] == "!exit") {
            if(whisper) {
                whisper = false;
                continue;
            }    
            host = "";
            break;
        }

        printr(client.getBufStr());
    }
    std::cout << "Server disconnected" << std::endl;
    std::exit(0);
}

int main(){
    client.init(true);

    std::thread input(inputHandler);
    std::thread incoming(incomingHandler);

    input.detach();
    incoming.join();
}
