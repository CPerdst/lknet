//
// Created by zwj1 on 24-12-4.
//

/**
 * 代码使用 Client 类创建客户端对象。
 * start 函数开启客户端对象，send 函数-
 * 可以发送数据到已连接的服务器。
 */

#include "Client.h"
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
        port = 18080;
    } else if (argc == 2) {
        host = argv[1];
        port = 18080;
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
    Client c(host, port);
    // 让 Client 运行在新线程之中，需要保证主线程一直运行
    c.start(true);
    while (true) {
        c.send("hello server");
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}