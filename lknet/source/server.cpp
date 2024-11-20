//
// Created by zwj1 on 2024/11/12.
//

#include "../include/server.h"
#include "eventCapturer.h"
#include "session.h"

namespace lknet{
    lknet::server::server(boost::asio::ip::address& addr, unsigned short port): io_ctx_(new boost::asio::io_context()),
                                                                                acceptor_(new boost::asio::ip::tcp::acceptor(*io_ctx_, boost::asio::ip::tcp::endpoint(addr, port)))
    {

    }

    void lknet::server::run() {
        do_async_accept();
        RootInfo() << "server is running at " << acceptor_->local_endpoint().address().to_string() << ":" << acceptor_->local_endpoint().port();
        io_ctx_->run();
    }

    void lknet::server::do_async_accept() {
        auto self(shared_from_this());
        acceptor_->async_accept(io_ctx_->get_executor(), [this, self](std::error_code ec, boost::asio::ip::tcp::socket sock){
            if(!ec){
                std::make_shared<session>(sock)->run();
                do_async_accept();
            }else{
                throw std::runtime_error(std::string("error code: ") + \
            std::to_string(ec.value()) + \
            std::string(" with") + \
            std::string(ec.message()));
            }
        });
    }
}

