//
// Created by zwj1 on 24-12-4.
//

/**
 * simple server with callback
 * 此样例展示了通过自定义 server 实例的 message callback 实现
 * 自定义 message 处理的功能
 */

#include "Server.h"
#include "iostream"

void initLogger() {
    logger::logger::Root()->setLevel(packer::Debug);
    logger::logger::Root()->setLogFormater(
        "[%level] [%s {%Y-%m-%d %H-%M-%S}]: %message\n");
}

int main(int argc, char *argv[]) {
    initLogger();
    // connect to 127.0.0.1 : 8080
    std::string host;
    unsigned short port = 0;
    if (argc == 1) {
        host = "127.0.0.1";
        port = 8080;
    } else if (argc == 2) {
        host = argv[1];
        port = 8080;
    } else if (argc == 3) {
        host = argv[1];
        port = std::stoi(argv[2]);
        if (!port) {
            RootError() << "argument 'port' parse failed.";
            exit(EXIT_FAILURE);
        }
    } else {
        RootError() << "usage: " << argv[0] << " [host] [port]";
    }

    Server s(host, port);
    std::function<void(Message)> handler = [](Message message) {
        // handle message
        // ...
        RootInfo() << "handled message.";
        // output handled message
        std::string s_msg = message.parseMessageToString();
        RootInfo() << s_msg;
    };
    s.setMessageHandler(handler);
    s.start();

    return 0;
}