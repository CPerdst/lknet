//
// Created by maziyang on 25-3-25.
//

#ifndef SERVERBUILDER_H
#define SERVERBUILDER_H

#include "Server.h"

class ServerBuilder : public std::enable_shared_from_this<ServerBuilder> {
public:
    std::string host{"127.0.0.1"};
    unsigned short port{9831};
    std::function<void(Message, IOBase *)> callback{nullptr};

    std::shared_ptr<ServerBuilder> setHost(const std::string& host);
    std::shared_ptr<ServerBuilder> setPort(unsigned short port);
    std::shared_ptr<ServerBuilder> setCallback(const std::function<void(Message, IOBase *)>& callback);
    std::shared_ptr<Server> build();
};

#endif //SERVERBUILDER_H
