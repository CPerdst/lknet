//
// Created by zwj1 on 24-11-27.
//

#ifndef GPT_NET_SERVER_H
#define GPT_NET_SERVER_H

#include <functional>
#include <memory>
#include <boost/asio.hpp>
#include <iostream>
#include "IOBase.h"
#include "tools.h"

class DLL_API Server {
public:
    Server(const std::string& host, unsigned short port, std::function<void(Message)> handler = nullptr);
    ~Server() = default;

    void start(bool runInOtherThread = false);

    void setMessageHandler(std::function<void(Message)> handler);

private:

    void doAccept();

    boost::asio::io_context ioContext;
    boost::asio::ip::tcp::acceptor acceptor;

    std::vector<std::shared_ptr<IOBase>> ioBases;

    std::mutex ioBasesMutex;

    std::function<void(Message)> messageHandler;

    bool runInOtherThread{};

};


#endif //GPT_NET_SERVER_H
