//
// Created by maziyang on 25-3-25.
//

#ifndef CLIENTBUILDER_H
#define CLIENTBUILDER_H
#include <memory>
#include "Client.h"

class ClientBuilder : public std::enable_shared_from_this<ClientBuilder> {
public:
    std::string host;
    unsigned short port;

    std::shared_ptr<ClientBuilder> setHost(const std::string& host);
    std::shared_ptr<ClientBuilder> setPort(unsigned short port);

    std::shared_ptr<Client> build();
};

#endif //CLIENTBUILDER_H
