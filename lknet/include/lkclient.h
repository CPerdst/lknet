//
// Created by zwj1 on 24-11-18.
//

#ifndef CLIENT_LKCLIENT_H
#define CLIENT_LKCLIENT_H

#include "iostream"

namespace lknet {

    class lkclient {
    public:
        lkclient() = default;
        virtual ~lkclient() = default;

        virtual void run() = 0;
        virtual void stop() = 0;

        virtual void deliver(std::string message) = 0;

    };

} // lknet

#endif //CLIENT_LKCLIENT_H
