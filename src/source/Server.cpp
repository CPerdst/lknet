//
// Created by zwj1 on 24-11-27.
//

#include "Server.h"

// -------------------------
// RequestHandlerRouter 实现
// -------------------------

RequestHandlerRouter RequestHandlerRouter::instance;

RequestHandlerRouter &RequestHandlerRouter::getInstance() {
    return instance;
}

void RequestHandlerRouter::registerHandlerGetter(unsigned short id, RequestHandlerRouter::HandlerGetterWithoutResponse getter) {
    std::unique_lock<std::mutex> lock(instanceMutex);
    mapper[id] = std::move(getter);
}

RequestHandlerRouter::HandlerGetterWithoutResponse& RequestHandlerRouter::get(unsigned short id) {
    std::unique_lock<std::mutex> lock(instanceMutex);
    auto it = mapper.find(id);
    if(it != mapper.end()){
        return it->second;
    }
    throw std::runtime_error("RequestHandlerRouter can't find(id: " + std::to_string(id) + ")");
}

// -------------------------
// Server 实现
// -------------------------

Server::Server(const std::string& host, unsigned short port, std::function<void(Message)> handler):
acceptor(ioContext),
messageHandler(std::move(handler)),
runInOtherThread(false)
{
    auto endpoint = boost::asio::ip::tcp::endpoint(
            boost::asio::ip::make_address(host),
            port);
    acceptor.open(endpoint.protocol());
    acceptor.set_option(boost::asio::socket_base::reuse_address(true));
    acceptor.bind(endpoint);
    acceptor.listen(10);
    RootInfo() << "Server Running on " << \
    endpoint.address().to_string() << \
    ":" << endpoint.port();
    // init default messageHandler
    if(handler){
        setMessageHandler(handler);
    }else{
        // 默认使用RequestHandlerRouter进行路由处理Request
        setMessageHandler([this](Message message){
            // 使用 RequestHandlerRouter 进行 Request-Handler 路由
            nlohmann::json requestJson = nlohmann::json::parse(message.parseMessageToString());
            Request r{};
            r.from_json(requestJson);
            auto handler = RequestHandlerRouter::getInstance().get(r.getId())();
            // 进行事务处理
            handler(r);
        });
    }
    doAccept();
}

void Server::start(bool runInOtherThread_) {
    runInOtherThread = runInOtherThread_;
    if(runInOtherThread){
        std::thread serverThread([this](){
            ioContext.run();
        });
        serverThread.detach();
    }else{
        ioContext.run();
    }
}

void Server::doAccept() {
    acceptor.async_accept([this](std::error_code ec, boost::asio::ip::tcp::socket sock){
        std::shared_ptr<IOBase> base = std::make_shared<IOBase>(sock);
        {   // 对ioBases操作需要上锁
            std::unique_lock<std::mutex> lock(ioBasesMutex);
            ioBases.push_back(base);
            auto weakBase = std::weak_ptr<IOBase>(base);
            ioBases.back()->setCloseHandler([weakBase, this]() {
                std::unique_lock<std::mutex> lock(ioBasesMutex);
                if (auto base = weakBase.lock()) { // 检查对象是否仍然存在
                    auto ite = std::find_if(ioBases.begin(), ioBases.end(), [&base](const std::shared_ptr<IOBase>& ptr) {
                        return ptr.get() == base.get();
                    });
                    if (ite != ioBases.end()) {
                        RootDebug() << (*ite)->socket().remote_endpoint().address().to_string() <<\
                        ":" << (*ite)->socket().remote_endpoint().port() << " has been removed from ioBases";
                        (*ite)->socket().close();
                        ioBases.erase(ite); // 从 vector 中移除
                    }
                }
            });
            ioBases.back()->setMessageHandler(messageHandler);
        }
        base->start();
        doAccept();
    });
}

void Server::setMessageHandler(std::function<void(Message)> handler) {
    std::unique_lock<std::mutex> lock(ioBasesMutex);
    messageHandler = std::move(handler);
    for(const auto& ioBase : ioBases){
        ioBase->setMessageHandler(messageHandler);
    }
}
