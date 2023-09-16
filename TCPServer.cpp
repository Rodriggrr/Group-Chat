#include <iostream>
#include <algorithm>
#include "include/socket/socket.hpp"

class Connection {
    Client client;
public:
    Connection() = default;
    Connection(Client client);
    void run();
    void commands(std::string msg);
    void disconnect();
    Client getClient(){
        return client;
    }
    bool operator==(const Connection& c) const {
        return this->client == c.client;
    }
};

void inputHandler();
void broadcast(std::string msg, int toIgnore);

Socket server(LOCALHOST, 4002);
std::vector<std::shared_ptr<Connection>> connections;


int main(){
    server.init();
    std::cout << "Servidor iniciado. Digite !help para ajuda.\n\n";

    std::thread input(inputHandler);
    input.detach();

    while(true) {
        auto client = server.accept();
        printr("Client " + client.getIpAddress() + ":" + std::to_string(client.getPort()) + " connecting..."); 
        auto connection = std::make_shared<Connection>(client);
        connections.push_back(connection);
    }

}

Connection::Connection(Client client){
    this->client = client;
    std::thread t(&Connection::run, this);
    t.detach();
}

void Connection::run(){
    int bytes = 1; 
    client.send("Escreva um nome de usuário ou deixe em branco para ser anônimo: ");
    bytes = client.recv();
    if(bytes > 1)
        client.setHost(client.getBufStr());
    client.send("!host " + client.getHost());
    auto connectedMsg = "Client connected: " + client.getIpAddress() + ":" + std::to_string(client.getPort()) + " (" + client.getHost() + ")";
    printr(connectedMsg);
    broadcast(horaAtual() + connectedMsg, client.getSocketFd());

    while(true){
        bytes = client.recv();
        if(bytes < 1) break;
        if(client.isConnected() == false) return;
        if(client.getBufStr()[0] == '!'){ 
            commands(client.getBufStr());
            continue;
        }

        auto broadcastMsg = client.getHost() + ": " + client.getBufStr();
        broadcast(horaAtual() + broadcastMsg, client.getSocketFd());


        
        printr(horaAtual() + client.getHost() + ": " + client.getBufStr());
    }
    disconnect();
}


void Connection::disconnect(){
    std::cout << "Client disconnected: " << client.getIpAddress() << ":" << client.getPort() << " (" << client.getHost() << ")" << std::endl;
    
    auto it = std::find_if(connections.begin(), connections.end(), [&](auto c){
        return c->getClient() == client;
    });
    if(it != connections.end()){
        client.setConnected(false);
        it->reset();
        connections.erase(it);
    }
}

void Connection::commands(std::string msg){
    auto args = fn::split(msg, ' ');

    if(args[0] == "!list") {
        std::string list = "Usuários conectados:\n";
        for (int i = 0; i < connections.size(); i++){
            list += std::to_string(i) + ": " + connections[i]->getClient().getIpAddress() + ":" + std::to_string(connections[i]->getClient().getPort()) + " (" + connections[i]->getClient().getHost() + ")\n";
        }
        client.send(list);
    
    } 
    else if (args[0] == "!exit") disconnect();

    else if (args[0] == "!whisper"){
        auto index = fn::number(args[1]);
        if(index > connections.size()-1) {
            client.send("Usuário não encontrado");
            return;
        }

        auto dest = std::make_shared<Client>(connections[index]->getClient());

        printr(client.getHost() + " está sussurrando para " + dest->getHost() + ". shhhh...");

        std::string pvMsg = "";
        client.send("!whisper");
        while(true) {
            auto bytes = client.recv();
            pvMsg = client.getBufStr();

            if(bytes < 1 or pvMsg == "!exit") break;

            dest->send(horaAtual() + client.getHost() + "(sussurro): " + pvMsg);
        }
        client.send("!exit");
        printr(client.getHost() + " parou de sussurrar.");
    }
    else if(args[0] == "!host"){
        args.erase(args.begin());
        client.setHost(fn::join(args, " "));
        client.send("!host " + client.getHost());
    }
    else if(args[0] == "!help"){
        std::stringstream help; 
        help << "Comandos disponíveis:\n"
             << "!list               - lista os usuários conectados.\n"
             << "!whisper <index>    - entra em modo de mensagem privada.\n"
             << "!host <name>        - define um nome de usuário.\n"
             << "!exit               - sai do programa ou do modo whisper.\n"
             << "!help               - mostra essa mensagem.\n";
        client.send(help.str());
    }
}

void broadcast(std::string msg, int toIgnore=-1){
    for(auto& c : connections){
        if(c->getClient().getSocketFd() != toIgnore){
            c->getClient().send(msg);
        }
    }
}

#include "include/inputHandler.hpp"