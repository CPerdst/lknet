// main.cpp

#include <iostream>
#include <thread>
#include "Server.h"

//void initLogger(){
//    logger::logger::Root()->setLogFormater()
//}


int main(int argc, char* argv[]) {
    Server server("0.0.0.0", 8080);
    server.start();

    return 0;
}