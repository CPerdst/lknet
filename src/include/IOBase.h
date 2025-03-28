// iobase.h

#ifndef IOBASE_H
#define IOBASE_H

#include <boost/asio.hpp>
#include <deque>
#include <functional>
#include <mutex>

#include "Message.h"
#include "EventCapturer.h"
#include "tools.h"

class Client;
class Server;

class DLL_API IOBase : public std::enable_shared_from_this<IOBase> {
   public:
    friend Client;
    friend Server;

    explicit IOBase(boost::asio::io_context &io_context);
    explicit IOBase(boost::asio::ip::tcp::socket &socket);
    ~IOBase() = default;

    void start();
    void send(const Message &msg);
    void send(Message &&msg);
    void setMessageHandler(
        const std::function<void(Message, IOBase *)> &handler);
    void setCloseHandler(const std::function<void(void)> &handler);

   protected:
    boost::asio::ip::tcp::socket &socket();

   private:
    void doReadHeader();
    void doReadBody();
    void doWrite();

    boost::asio::ip::tcp::socket sock;
    Message readMessage;

    std::deque<Message> writeMessages;
    std::mutex writeMutex;
    std::function<void(Message, IOBase *)> messageHandler;

    std::function<void(void)> closeHandler;

    bool isIOBaseRunning;
};

#endif  // IOBASE_H