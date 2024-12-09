// client.cpp

#include "Client.h"
#include <iostream>

Client::Client(const std::string& host, unsigned short port):
resolver(ioContext),
ioBase(std::make_shared<IOBase>(ioContext)),
runInOtherThread(false)
{
    ioBase->setMessageHandler([this](Message message, IOBase*){
        std::string messageString = message.parseMessageToString();
        std::cout << messageString << std::endl;
    });

    ioBase->setCloseHandler([this](){
        close();
    });

    resolver.async_resolve(host, std::to_string(port),
                           [this](std::error_code ec, boost::asio::ip::tcp::resolver::results_type endpoints){
        if(!ec){
            do_connect(endpoints);
        }else{
            std::cout << "resolve host::service failed" << std::endl;
        }
    });
}

void Client::start(bool runInOtherThread_) {
    runInOtherThread = runInOtherThread_;
    if(runInOtherThread){
        std::thread contextThread([this](){
            ioContext.run();
        });
        contextThread.detach();
    }else{
        ioContext.run();
    }
}

void Client::do_connect(const boost::asio::ip::tcp::resolver::results_type& endpoints) {
    boost::asio::async_connect(ioBase->socket(), endpoints,
    [this](boost::system::error_code ec, const boost::asio::ip::tcp::endpoint& /*endpoint*/) {
       if (!ec) {
           ioBase->start();
       }else {
           std::cerr << "Connect failed: " << ec.message() << std::endl;
       }
    });
}

void Client::send(const std::string& message) {
    Message msg;
    msg.buildMessageFromString(message);
    ioBase->send(msg);
}

void Client::send(const Request& request) {
    Message msg;
    nlohmann::json serialization_request_json = request.to_json();
    msg.buildMessageFromString(serialization_request_json.dump());
    ioBase->send(msg);
}

void Client::close() {
    ioBase->socket().close();
    ioContext.stop();
    std::cout << "closed connection to server" << std::endl;
}