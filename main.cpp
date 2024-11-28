// main.cpp

#include <iostream>
#include <thread>
#include "Client.h"
#include "Server.h"


void printClass(){
    using socket_t = std::enable_shared_from_this<IOBase>;
    std::cout << std::boolalpha;
    std::cout << "Default constructible: " << std::is_default_constructible<socket_t>::value << '\n';
    std::cout << "Copy constructible: " << std::is_copy_constructible<socket_t>::value << '\n';
    std::cout << "Move constructible: " << std::is_move_constructible<socket_t>::value << '\n';
    std::cout << "Copy assignable: " << std::is_copy_assignable<socket_t>::value << '\n';
    std::cout << "Move assignable: " << std::is_move_assignable<socket_t>::value << '\n';
}


int main(int argc, char* argv[]) {
    printClass();

    Client client("127.0.0.1", 8080);
    client.start(true);

    Server server("0.0.0.0", 8080);
    server.start(true);


    int times = 5;
    while(times++){
        client.send("nihaoaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}