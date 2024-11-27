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

class Server {
public:
    Server(const std::string& host, unsigned short port);
    ~Server() = default;

    void start(bool runInOtherThread = false);

private:

    void doAccept();

    boost::asio::io_context ioContext;
    boost::asio::ip::tcp::acceptor acceptor;

    std::vector<std::shared_ptr<IOBase>> ioBases;

    std::function<void(void)> callback;

    bool runInOtherThread{};

};


#endif //GPT_NET_SERVER_H
