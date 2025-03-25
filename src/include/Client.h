// client.h

#ifndef CLIENT_H
#define CLIENT_H

#include <boost/asio.hpp>
#include <memory>

#include "IOBase.h"
#include "common.h"
#include "queue"
#include "tools.h"

class ClientBuilder;

class DLL_API Client {
   public:
    // 使用临时构造的Response，然后通过Handler延长Response寿命周期供客户端使用
    using ResponseHandler = std::function<void(const Response &)>;

    // 默认连接 127.0.0.1:9831
    Client();

    Client(std::shared_ptr<ClientBuilder> builder);

    // 解析 configPath
    Client(const std::string& configPath);

    Client(const std::string &host, unsigned short port);

    void start(bool runInOtherThread = false);

    /**
     * 发送Request
     *
     * @param Request 请求结构体
     * @param ResponseHandler 用于处理服务器发来的响应
     */

    void send(const Request &, ResponseHandler &&);

    void send(const Request &, const ResponseHandler &);

    /**
     * 发送string字符串
     * @param message
     *
     * @note 已废弃
     */
    void send(const std::string &message);

    void close();

   private:
    void do_connect(
        const boost::asio::ip::tcp::resolver::results_type &endpoints);

    boost::asio::io_context ioContext;
    boost::asio::ip::tcp::resolver resolver;
    std::shared_ptr<IOBase> ioBase;

    // 响应Handler队列
    std::queue<ResponseHandler> handlers;  // 临界资源

    std::mutex handlersMutex;

    bool runInOtherThread;

    std::string configPath;
    std::string host;
    std::string port;
};

#endif  // CLIENT_H