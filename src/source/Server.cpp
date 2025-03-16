//
// Created by zwj1 on 24-11-27.
//

#include "Server.h"

#include "ConfigLoader.h"

using namespace lknet::util;

// -------------------------
// RequestHandlerRouter 实现
// -------------------------

RequestHandlerRouter RequestHandlerRouter::instance;

RequestHandlerRouter &RequestHandlerRouter::getInstance() { return instance; }

void RequestHandlerRouter::registerHandlerGetter(
    unsigned short id,
    RequestHandlerRouter::HandlerGetterWithoutResponse getter) {
    std::unique_lock<std::mutex> lock(instanceMutex);
    getMapper()[id] =
        std::variant<HandlerGetterWithoutResponse, HandlerGetterWithResponse>(
            getter);
}

void RequestHandlerRouter::registerHandlerGetterWithResponse(
    unsigned short id, RequestHandlerRouter::HandlerGetterWithResponse getter) {
    std::unique_lock<std::mutex> lock(instanceMutex);
    getMapper()[id] =
        std::variant<HandlerGetterWithoutResponse, HandlerGetterWithResponse>(
            getter);
}

std::map<unsigned short, std::variant<RequestHandlerRouter::HandlerGetterWithoutResponse,
RequestHandlerRouter::HandlerGetterWithResponse>> RequestHandlerRouter::getMapper() {
    static std::map<unsigned short, std::variant<HandlerGetterWithoutResponse, HandlerGetterWithResponse>> mapper;
    return mapper;
}

std::variant<RequestHandlerRouter::HandlerGetterWithoutResponse,
             RequestHandlerRouter::HandlerGetterWithResponse> &
RequestHandlerRouter::get(unsigned short id) {
    std::unique_lock<std::mutex> lock(instanceMutex);
    auto it = getMapper().find(id);
    if (it != getMapper().end()) {
        return it->second;
    }
    throw std::runtime_error(
        "RequestHandlerRouter can't find(id: " + std::to_string(id) + ")");
}

// -------------------------
// Server 实现
// -------------------------

Server::Server()
    : acceptor(ioContext), messageHandler(nullptr), runInOtherThread(false) {
    auto &configLoader = ConfigLoader::getInstance();
    configLoader.loadConfig("config.json");
    auto &configMap = configLoader.getConfig();
    auto hostIt = configMap.find("host");
    auto portIt = configMap.find("port");
    init(hostIt == configMap.end() ? "127.0.0.1" : hostIt->second,
         portIt == configMap.end() ? 8080 : std::stoi(portIt->second), nullptr);
}

Server::Server(const std::string &host, unsigned short port,
               std::function<void(Message, IOBase *)> handler)
    : acceptor(ioContext),
      messageHandler(std::move(handler)),
      runInOtherThread(false) {
    init(host, port, handler);
}

void Server::start(bool runInOtherThread_) {
    runInOtherThread = runInOtherThread_;
    if (runInOtherThread) {
        std::thread serverThread([this]() { ioContext.run(); });
        serverThread.detach();
    } else {
        ioContext.run();
    }
}

void Server::init(const std::string &host, unsigned short port,
                  std::function<void(Message, IOBase *)> handler) {
    auto endpoint = boost::asio::ip::tcp::endpoint(
        boost::asio::ip::make_address(host), port);
    acceptor.open(endpoint.protocol());
    acceptor.set_option(boost::asio::socket_base::reuse_address(true));
    acceptor.bind(endpoint);
    acceptor.listen(10);
    RootInfo() << "Server Running on " << endpoint.address().to_string() << ":"
               << endpoint.port();
    // init default messageHandler
    if (handler) {
        setMessageHandler(handler);
    } else {
        // 默认使用RequestHandlerRouter进行路由处理Request
        setMessageHandler([this](Message message, IOBase *base) {
            // 使用 RequestHandlerRouter 进行 Request-Handler 路由
            nlohmann::json requestJson =
                nlohmann::json::parse(message.parseMessageToString());
            Request r{};
            r.from_json(requestJson);
            auto handlerVariant =
                RequestHandlerRouter::getInstance().get(r.getId());
            // 进行事务处理
            if (std::holds_alternative<
                    RequestHandlerRouter::HandlerGetterWithoutResponse>(
                    handlerVariant)) {
                auto creator = std::get<
                    RequestHandlerRouter::HandlerGetterWithoutResponse>(
                    handlerVariant);
                creator()(r);
            } else if (std::holds_alternative<
                           RequestHandlerRouter::HandlerGetterWithResponse>(
                           handlerVariant)) {
                auto creator =
                    std::get<RequestHandlerRouter::HandlerGetterWithResponse>(
                        handlerVariant);
                auto response = creator()(r);
                // 发送响应
                Message responseMessage;
                responseMessage.buildMessageFromString(
                    response.to_json().dump());
                base->send(responseMessage);
            }
        });
    }
    doAccept();
}

void Server::doAccept() {
    acceptor.async_accept([this](std::error_code ec,
                                 boost::asio::ip::tcp::socket sock) {
        std::shared_ptr<IOBase> base = std::make_shared<IOBase>(sock);
        {  // 对ioBases操作需要上锁
            std::unique_lock<std::mutex> lock(ioBasesMutex);
            ioBases.push_back(base);
            auto weakBase = std::weak_ptr<IOBase>(base);
            ioBases.back()->setCloseHandler([weakBase, this]() {
                std::unique_lock<std::mutex> lock(ioBasesMutex);
                if (auto base = weakBase.lock()) {  // 检查对象是否仍然存在
                    auto ite = std::find_if(
                        ioBases.begin(), ioBases.end(),
                        [&base](const std::shared_ptr<IOBase> &ptr) {
                            return ptr.get() == base.get();
                        });
                    if (ite != ioBases.end()) {
                        RootDebug()
                            << (*ite)
                                   ->socket()
                                   .remote_endpoint()
                                   .address()
                                   .to_string()
                            << ":" << (*ite)->socket().remote_endpoint().port()
                            << " has been removed from ioBases";
                        (*ite)->socket().close();
                        ioBases.erase(ite);  // 从 vector 中移除
                    }
                }
            });
            ioBases.back()->setMessageHandler(messageHandler);
        }
        base->start();
        doAccept();
    });
}

void Server::setMessageHandler(std::function<void(Message, IOBase *)> handler) {
    std::unique_lock<std::mutex> lock(ioBasesMutex);
    messageHandler = std::move(handler);
    for (const auto &ioBase : ioBases) {
        ioBase->setMessageHandler(handler);
    }
}
