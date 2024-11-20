#include <iostream>
#include "server.h"
#include "client.h"
#include "boost/asio.hpp"
#include "eventCapturer.h"

void init_logger(){
    logger::logger::Root()->setLevel(packer::Trace);
    logger::logger::Root()->setLogFormater("[%level] [%s {%Y-%m-%d %H:%M:%S}]: %message\n");
}

int main(int argc, char** argv) {
    init_logger();
    boost::asio::ip::address addr;
    unsigned short port = 0;
    if(argc > 2){
        if(strncmp(argv[1], "client", 6) == 0){
            RootInfo() << "================== LKCLIENT ==================";
            if(argc != 4){
                goto end;
            }
            try{
                addr = boost::asio::ip::make_address(argv[2]);
                port = std::strtol(argv[3], nullptr, 10);
                if(port <= 0 || port > 65535){
                    throw std::runtime_error("port parse failed");
                }
            }catch(std::exception& e){
                RootError() << "error occur: " << e.what();
                exit(0);
            }

            try{
                std::shared_ptr<lknet::client> c = std::make_shared<lknet::client>(addr, port);
                c->run();
                while(true){
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    c->deliver("aaaaaaaaaaaaaaaaaaaaaa");
                }
            }catch(std::exception& e){
                RootError() << e.what();
            }
        }
        else if(strncmp(argv[1], "server", 6) == 0){
            RootInfo() << "================== LKSERVER ==================";
            if(argc != 4){
                goto end;
            }
            try{
                addr = boost::asio::ip::make_address(argv[2]);
                port = std::strtol(argv[3], nullptr, 10);
            }catch(std::exception& e){
                RootWarn() << "use default 0.0.0.0:8080 to run the server: " << e.what();
                addr = boost::asio::ip::make_address("0.0.0.0");
                port = 8080;
            }
            try{
                std::make_shared<lknet::server>(addr, port)->run();
            }catch(std::exception& e){
                RootError() << "server crashed with: " << e.what();
                exit(0);
            }
        }
    }
    end:
        RootInfo() << "use example: " << argv[0] << " [client/server] addr:port/port";

    return 0;
}
