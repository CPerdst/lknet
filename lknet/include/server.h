//
// Created by zwj1 on 2024/11/12.
//

#ifndef LKNET_SERVER_H
#define LKNET_SERVER_H

#include "lkserevr.h"
#include "memory"
#include "boost/asio.hpp"

namespace lknet {
    /**
     * @brief 抽象服务器类
     *
     * 用于实现服务器的基本功能：异步接收连接、异步读取消息、异步处理任务、异步发送消息
     *
     */
class server: public  lkserver, public std::enable_shared_from_this<server>{
    public:
        server() = delete;
        ~server() override = default;
        server(boost::asio::ip::address& addr, unsigned short port);

        void run() override;

    private:
        void do_async_accept();

        std::shared_ptr<boost::asio::io_context> io_ctx_;
        std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor_;
    };
}

#endif //LKNET_SERVER_H
