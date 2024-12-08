// client.h

#ifndef CLIENT_H
#define CLIENT_H

#include <boost/asio.hpp>
#include <memory>
#include "IOBase.h"
#include "tools.h"
#include "common.h"

class DLL_API Client {
public:
    Client(const std::string& host, unsigned short port);

    void start(bool runInOtherThread = false);
    void send(const std::string& message);
    void send(const Request& request);
    void close();

private:
    void do_connect(const boost::asio::ip::tcp::resolver::results_type& endpoints);

    boost::asio::io_context ioContext;
    boost::asio::ip::tcp::resolver resolver;
    std::shared_ptr<IOBase> ioBase;

    bool runInOtherThread;
};

#endif // CLIENT_H