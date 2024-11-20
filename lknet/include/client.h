//
// Created by zwj1 on 24-11-19.
//

#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H

#include "lkclient.h"
#include "boost/asio.hpp"
#include "message.h"
#include "queue"

namespace lknet {

class client: public lkclient, public std::enable_shared_from_this<client> {
    public:
        client() = delete;
        ~client() override = default;
        client(boost::asio::ip::address &addr, unsigned short port);

        void run() override;
        void stop() override;
        void deliver(std::string message) override;

    private:
        void do_async_read_header();
        void do_async_read_body();
        int do_handle_received_data();
        void do_async_send();
        bool client_net_running() const;
        void client_net_running(bool flag);

        boost::asio::io_context io_ctx_;
        boost::asio::ip::tcp::socket sock_;
        bool client_net_running_;
        common::message received_message_;
        boost::asio::ip::address& addr_;
        unsigned short port_;

        std::queue<common::message> ordered_message_queue_;
        std::mutex message_queue_mtx_;
    };

} // lknet

#endif //CLIENT_CLIENT_H
