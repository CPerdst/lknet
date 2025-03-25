//
// Created by maziyang on 25-3-25.
//

#include <memory>
#include "ServerBuilder.h"

std::shared_ptr<ServerBuilder> ServerBuilder::setHost(const std::string& host) {
    this->host = host;
    return shared_from_this();
}

std::shared_ptr<ServerBuilder> ServerBuilder::setPort(unsigned short port) {
    this->port = port;
    return shared_from_this();
}

std::shared_ptr<ServerBuilder> ServerBuilder::setCallback(const std::function<void(Message, IOBase *)>& callback) {
    this->callback = callback;
    return shared_from_this();
}

std::shared_ptr<Server> ServerBuilder::build() {
    return std::make_shared<Server>(shared_from_this());
}