inline void inputHandler(){
    auto indexHandler = [&](int index) -> bool {
        if (index < 0 || index >= connections.size()) {
            std::cout << "Invalid index" << std::endl;
            return false;
        }
        return true;
    };

    int focus = -1;
    while(true){
        if(focus != -1){
            if(focus > connections.size()-1) {
                focus = -1;
                continue;
            }
            std::cout << focus << ": ";
        }
        else
            std::cout << ">> ";

        std::string msg;
        getline(std::cin, msg);

        if(connections.size() == 0 and msg[0] != '!'){
            std::cout << "No clients connected" << std::endl;
            continue;
        }

        auto args = fn::split(msg, ' ');
        if(args[0] == "!list"){
            for (int i = 0; i < connections.size(); i++){
                std::cout << i << ": " << connections[i]->getClient().getIpAddress() << ":" << connections[i]->getClient().getPort() << " (" << connections[i]->getClient().getHost() << ") " << ping(connections[i]->getClient())  << "ms" << std::endl;
            }
        }

        else if(args[0] == "!focus"){
            int index = fn::number(args[1]);
            if(!indexHandler(index)) continue;

            focus = index;
        } 
        
        else if (args[0] == "!disconnect"){
            int index = fn::number(args[1]);
            if(!indexHandler(index)) continue;

            server.send("!disconnect", connections[index]->getClient().getSocketFd());
            connections[index]->disconnect();
        }

        else if (args[0] == "!help"){
            std::cout << "Comandos disponíveis:\n"
                      << "!list               - lista os usuários conectados.\n"
                      << "!disconnect <index> - desconecta um usuário. use !list para ver os índices.\n"
                      << "!focus <index>      - faz com que o padrao de envio seja para tal usuário.\n"
                      << "!exit               - sai do programa ou do modo de foco\n"
                      << "!help               - mostra essa mensagem.\n";
        }

        else if(args[0] == "!exit"){
            if(focus == -1)
                std::exit(0);
            else
                focus = -1;
        }

        else if(args[0] == "!broadcast"){
            args.erase(args.begin(), args.begin()+1);
            msg = fn::join(args, " ");

            
            broadcast("Server: " + msg);
        }

        else if(args[0][0] == '!'){
            std::cout << "Comando inválido" << std::endl;
        }

        else {
            if(focus != -1){
                server.send("Server: " + msg, connections[focus]->getClient().getSocketFd());
            } else {
                broadcast("Server: " + msg);
            }
        }

    }
}