# Group Chat in C++

<!-- toc -->

- [Sobre](#sobre)
- [Como usar](#como-usar)
  - [Compilando](#compilando)
- [Mídia](#mídia)

<!-- toc -->

## Sobre

É um projeto de um chat em grupo desenvolvido em C++ para a disciplina de Sistemas Distribuídos.
Você pode checar o projeto na pasta da disciplina [aqui](https://github.com/Rodriggrr/UFC/tree/main/2023.2/sd/lista_1)

## Como usar

Para compilar, você necessita ter instalado o `g++11` |  **[INSTALAR](https://github.com/senapk/fupisfun/blob/master/wiki/configure_cpp.md#configurando-o-compilador-gcc-g)**

Só funciona em ambientes baseados em **Unix**.

### Compilando

- **Server**

    ```bash
    g++ -std=c++20 -pthread TCPServer.cpp -o server
    ./server
    ```

- **Cliente**

    ```bash
    # Com o servidor já iniciado:
    g++ -std=c++20 -pthread TCPClient.cpp -o client
    ./client
    ```

## Mídia

![Server](include/midi/screenshot.png)
