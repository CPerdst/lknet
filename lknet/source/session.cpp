//
// Created by zwj1 on 2024/11/13.
//

#include "session.h"
#include "iostream"
#include "eventCapturer.h"

namespace lknet {
    session::session(boost::asio::ip::tcp::socket &sock): sock_(std::move(sock))
    {

    }

    void session::run() {
        RootInfo() << "client " << sock_.remote_endpoint().address().to_string() << ":" << sock_.remote_endpoint().port() << " connected";
        do_async_read_header();
    }

    void session::do_async_read_header() {
        auto self(shared_from_this());
        boost::asio::async_read(sock_, boost::asio::buffer(received_message_.header(), common::message::header_size),
                            [this, self](std::error_code ec, std::size_t size){
            int s = received_message_.parse_body_size();
            RootTrace() << "received header size: " << s;
            if(!ec && s > 0){
                do_async_read_body();
            }else{
                RootInfo() << "session " << sock_.remote_endpoint().address().to_string() << "/" << sock_.remote_endpoint().port() << " closed: " << ec.message();
                // 断开连接
            }
        });
    }

    void session::do_async_read_body() {
        auto self(shared_from_this());
        boost::asio::async_read(sock_, boost::asio::buffer(received_message_.body(), received_message_.size()),
                            [this, self](std::error_code ec, std::size_t size){
            RootTrace() << "received body: " << received_message_.body();
            if(!ec && size){
                // 处理接收到的数据
                do_handle_received_data();
                received_message_.init();
                do_async_read_header();
            }else{
                RootInfo() << "session " << sock_.remote_endpoint().address().to_string() << "/" << sock_.remote_endpoint().port() << " closed: " << ec.message();
                // 断开连接
            }
        });
    }

    void session::do_handle_received_data() {
        // 首先，将数据反序列化成为请求对象
//        std::string body(received_message_.body(), received_message_.size());
        std::cout << received_message_.body() << std::endl;
    }


} // lknet