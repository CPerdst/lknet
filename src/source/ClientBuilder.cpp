//
// Created by maziyang on 25-3-25.
//

#include "ClientBuilder.h"

#include <memory>
#include <string>
std::shared_ptr<ClientBuilder> ClientBuilder::setHost(const std::string& host) {
    this->host = host;
    return shared_from_this();
}

std::shared_ptr<ClientBuilder> ClientBuilder::setPort(unsigned short port) {
    this->port = port;
    return shared_from_this();
}
std::shared_ptr<Client> ClientBuilder::build() {
    return std::make_shared<Client>(shared_from_this());
}