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
#include "common.h"
#include "variant"

class DLL_API RequestHandlerRouter {
public:
    using HandlerWithoutResponse = std::function<void(const Request&)>;
    using HandlerWithResponse = std::function<Response(const Request&)>;
    using HandlerGetterWithoutResponse = std::function<HandlerWithoutResponse()>;
    using HandlerGetterWithResponse = std::function<HandlerWithResponse()>;

    RequestHandlerRouter(const RequestHandlerRouter&) = delete;
    RequestHandlerRouter& operator=(const RequestHandlerRouter&) = delete;

    static RequestHandlerRouter &getInstance();

    void registerHandlerGetter(unsigned short id, HandlerGetterWithoutResponse getter);

    void registerHandlerGetterWithResponse(unsigned short id, HandlerGetterWithResponse getter);

    std::variant<RequestHandlerRouter::HandlerGetterWithoutResponse, \
    RequestHandlerRouter::HandlerGetterWithResponse>& \
    get(unsigned short id);

private:
    RequestHandlerRouter() = default;
    ~RequestHandlerRouter() = default;

    std::map<unsigned short, std::variant<HandlerGetterWithoutResponse, HandlerGetterWithResponse>> mapper;

    static RequestHandlerRouter instance;

    std::mutex instanceMutex;
};

class DLL_API Server {
public:
    Server();
    Server(const std::string&, unsigned short, std::function<void(Message, IOBase*)> handler = nullptr);
    ~Server() = default;

    void start(bool runInOtherThread = false);

    void setMessageHandler(std::function<void(Message, IOBase*)> handler);

private:
    void init(const std::string&, unsigned short, std::function<void(Message, IOBase*)> handler);

    void doAccept();

    boost::asio::io_context ioContext;
    boost::asio::ip::tcp::acceptor acceptor;

    std::vector<std::shared_ptr<IOBase>> ioBases;

    std::mutex ioBasesMutex;

    std::function<void(Message, IOBase*)> messageHandler;

    bool runInOtherThread{};

};


#endif //GPT_NET_SERVER_H
