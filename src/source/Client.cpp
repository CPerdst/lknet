// client.cpp

#include "Client.h"
#include <iostream>
#include "ConfigLoader.h"

Client::Client()
: resolver(ioContext),
      ioBase(std::make_shared<IOBase>(ioContext)),
      runInOtherThread(false){
    Client("127.0.0.1", 9831);
}

Client::Client(const std::string& configPath)
: resolver(ioContext),
      ioBase(std::make_shared<IOBase>(ioContext)),
      runInOtherThread(false){
    lknet::util::ConfigLoader& loader = lknet::util::ConfigLoader::getInstance();
    loader.setConfigOptions({{"parseFormat", "json"}});
    loader.loadConfig(configPath);
    auto config = loader.getConfig();
    this->host = config["host"];
    this->port = config["port"];
    Client(host, std::stoi(port));
}

Client::Client(const std::string &host, unsigned short port)
    : resolver(ioContext),
      ioBase(std::make_shared<IOBase>(ioContext)),
      runInOtherThread(false) {
    ioBase->setMessageHandler([this](Message message, IOBase *basePtr) {
        ResponseHandler handler;
        // 反序列化Response对象
        Response response;
        std::string ss = message.parseMessageToString();
        RootTrace() << "Received message from "
                    << basePtr->sock.remote_endpoint().address().to_string()
                    << ":" << basePtr->sock.remote_endpoint().port() << " : "
                    << ss;
        response.from_json(nlohmann::json::parse(ss));
        // 获取队头handler，处理Response
        {
            std::unique_lock<std::mutex> lock(handlersMutex);
            handler = std::move(handlers.front());
            handlers.pop();
        }
        handler(response);
    });

    ioBase->setCloseHandler([this]() { close(); });

    this->host = host;
    this->port = std::to_string(port);
}

void Client::start(bool runInOtherThread_) {
    runInOtherThread = runInOtherThread_;
    resolver.async_resolve(
    host, port,
    [this](std::error_code ec,
           boost::asio::ip::tcp::resolver::results_type endpoints) {
        if (!ec) {
            do_connect(endpoints);
        } else {
            std::cout << "resolve host::service failed" << std::endl;
            exit(1);
        }
    });
    if (runInOtherThread) {
        std::thread contextThread([this]() { ioContext.run(); });
        contextThread.detach();
    } else {
        ioContext.run();
    }
}

void Client::do_connect(
    const boost::asio::ip::tcp::resolver::results_type &endpoints) {
    boost::asio::async_connect(
        ioBase->socket(), endpoints,
        [this](boost::system::error_code ec,
               const boost::asio::ip::tcp::endpoint & /*endpoint*/) {
            if (!ec) {
                ioBase->start();
            } else {
                std::cerr << "Connect failed: " << ec.message() << std::endl;
            }
        });
}

void Client::send(
    const Request &request,
    const ResponseHandler &handler = [](const Response &) { return; }) {
    Message msg;
    nlohmann::json serialization_request_json = request.to_json();
    msg.buildMessageFromString(serialization_request_json.dump());
    {
        std::unique_lock<std::mutex> lock(handlersMutex);
        handlers.push(handler);
    }
    ioBase->send(msg);
}

void Client::send(
    const Request &request,
    ResponseHandler &&handler = [](const Response &) { return; }) {
    Message msg;
    nlohmann::json serialization_request_json = request.to_json();
    msg.buildMessageFromString(serialization_request_json.dump());
    {
        std::unique_lock<std::mutex> lock(handlersMutex);
        handlers.push(std::move(handler));
    }
    ioBase->send(msg);
}

void Client::send(const std::string &message) {
    std::unique_lock<std::mutex> lock(handlersMutex);
    Message msg;
    msg.buildMessageFromString(message);
    ioBase->send(msg);
}

void Client::close() {
    ioBase->socket().close();
    ioContext.stop();
    std::cout << "closed connection to server" << std::endl;
}