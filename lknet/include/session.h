//
// Created by zwj1 on 2024/11/13.
//

#ifndef LKNET_SESSION_H
#define LKNET_SESSION_H

#include "boost/asio.hpp"
#include "message.h"

namespace lknet {

    class session: public std::enable_shared_from_this<session> {
    public:
        explicit session(boost::asio::ip::tcp::socket& sock);
        ~session() = default;

        void run();

    private:
        void do_async_read_header();
        void do_async_read_body();
        void do_handle_received_data();

        boost::asio::ip::tcp::socket sock_;
        lknet::common::message received_message_;
    };

} // lknet

#endif //LKNET_SESSION_H
