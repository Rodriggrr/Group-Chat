#ifndef COMMON
    #include "../_common.hpp"
#endif

class Socket {
    int socket_fd;
    sockaddr_in addr;
    int port=7896;
    std::string host=LOCALHOST0;
    char buffer[4096];

public:
    Socket() = default;

    Socket(auto host=LOCALHOST0, auto port=7896) {
        this->host = host;
        this->port = port;
    }


    int init(bool isClient=false){

        try {
            this->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = inet_addr(host.c_str());
        } catch(...) {
            std::cout << "Error creating socket" << std::endl;
            return -1;
        }

        if(!isClient){
             int opt = 1;

            if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
                perror("setsockopt");


            if(bind(socket_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
                std::cout << "Can't bind to IP/Port" << std::endl;
                return -1;
            }

            if(listen(socket_fd, 10) < 0) {
                std::cout << "Can't listen" << std::endl;
                return -1;
            }
        } else {
             while(connect(socket_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
                std::cout << "Impossível conectar. Digite uma nova combinação IP:PORTA, ou deixe em branco para sair. (Porta padrão: 49100)" << std::endl;
                std::string ipPorta;
                std::getline(std::cin, ipPorta);
                if(ipPorta.size() > 0) {
                    auto args = fn::split(ipPorta, ':');
                    if(args.size() == 2) {
                        addr.sin_addr.s_addr = inet_addr(args[0].c_str());
                        addr.sin_port = htons(std::stoi(args[1]));
                    }
                }
                else break;
            }
            return -1;
        }


        return 0;
    }

    Client accept(){
        Client c;
        
        c.setSocketFd(::accept(socket_fd, (sockaddr *)c.getAddr(), (socklen_t*)c.getAddr()));

        return c;
    }

    int getPort(){
        return port;
    }

    std::string getBufStr(){
        return std::string(buffer);
    }

    int getSocketFd(){
        return socket_fd;
    }

    char* getBuffer(){
        return buffer;
    }

    int close(){
        return ::close(socket_fd);
    }

    int recv(int socket_fd=-1){
        memset(buffer, '\0', 4096);
        if(socket_fd == -1) socket_fd = this->socket_fd;

        auto bytes = ::recv(socket_fd, buffer, 4096, 0);
        if(bytes < 0) {
            std::cout << "Error receiving data" << std::endl;
            return -1;
        }
        return bytes;
    }

    int send(std::string data, int socket_fd=-1){
        if(socket_fd == -1) socket_fd = this->socket_fd;

        auto bytes = ::send(socket_fd, data.c_str(), data.size(), 0);
        if(bytes < 0) {
            std::cout << "Error sending data" << std::endl;
            return -1;
        }
        return bytes;
    }
};