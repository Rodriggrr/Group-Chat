#ifndef COMMON
    #include "../_common.hpp"
#endif

class Socket {
    SOCKET socket_fd;
    sockaddr_in addr;
    int port;
    std::string host=LOCALHOST0;
    char buffer[4096];

public:
    Socket() = default;

    Socket(auto host=LOCALHOST, auto port=49100) {
        this->host = host;
        this->port = port;
    }


    int init(bool isClient=false){

         // Inicializar a biblioteca Winsock
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "Falha ao inicializar o Winsock." << std::endl;
            return 1;
        }

        try {
            socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

            if (socket_fd == INVALID_SOCKET) {
                std::cerr << "Erro ao criar o socket." << std::endl;
                WSACleanup();
                return 1;
            }

            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);

            if(isClient){
                char* hostHeap = new char[host.size()+1];
                strcpy(hostHeap, host.c_str());
                addr.sin_addr.s_addr = inet_addr(hostHeap);
                delete[] hostHeap;
            }
            else {
                addr.sin_addr.s_addr = INADDR_ANY;
            }

        } catch(...) {
            std::cout << "Error creating socket" << std::endl;
            return -1;
        }

        if(!isClient){
            int opt = 1;

            setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&opt), sizeof(opt));

            //bind winsock
            while (bind(socket_fd, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
                int error = WSAGetLastError();
                std::cerr << "Erro ao vincular o socket: " << error << std::endl;
                std::cout << "Digite uma nova combinação de IP e porta, deixe em branco para usar a padrão., ou -1 na porta para sair." << std::endl;
                std::cout << "IP: ";
                std::string ip;
                std::getline(std::cin, ip);
                if(ip.size() > 0) {
                    char* hostHeap = new char[ip.size()+1];
                    strcpy(hostHeap, ip.c_str());
                    addr.sin_addr.s_addr = inet_addr(hostHeap);
                    delete[] hostHeap;
                }
                else {
                    addr.sin_addr.s_addr = INADDR_ANY;
                }
                std::cout << "Porta: ";
                std::string port;
                std::getline(std::cin, port);
                if(port == "-1") exit(0);

                else if(port.size() > 0) {
                    this->port = std::stoi(port);
                    addr.sin_port = htons(this->port);
                }
                else {
                    this->port = 49100;
                    addr.sin_port = htons(this->port);
                }
                closesocket(socket_fd);
                WSACleanup();
                return 1;
            }

            if (listen(socket_fd, 5) == SOCKET_ERROR) {
                std::cerr << "Erro ao colocar o socket em modo de escuta." << std::endl;
                closesocket(socket_fd);
                WSACleanup();
                return 1;
            }

        } else {
            if(connect(socket_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
                std::cout << "Can't connect to server" << std::endl;
                return -1;
            }
        }
        return 0;
    }

    Client accept(){
        Client c;
        
        c.setSocketFd(::accept(socket_fd, (sockaddr *)c.getAddr(), c.getAddrLen()));

        return c;
    }

    sockaddr_in* getAddrPtr(){
        return &addr;
    }

    sockaddr_in getAddr(){
        return addr;
    }

    int getAddrLen(){
        return sizeof(addr);
    }

    std::string getBufStr(){
        return std::string(buffer);
    }

    SOCKET getSocketFd(){
        return socket_fd;
    }

    char* getBuffer(){
        return buffer;
    }

    int close(){
        return closesocket(socket_fd);
    }

    int recv(SOCKET socket_fd=-1){
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