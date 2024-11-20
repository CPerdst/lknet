//
// Created by zwj1 on 24-11-19.
//

#include "client.h"
#include "eventCapturer.h"

namespace lknet {
    client::client(boost::asio::ip::address &addr, unsigned short port):
    sock_(io_ctx_),
    client_net_running_(false),
    addr_(addr),
    port_(port)
    {

    }

    void client::run(){
        if(!client_net_running()){
            auto self(shared_from_this());
            client_net_running(true);
//            boost::asio::make_work_guard(io_ctx_);
            boost::asio::ip::tcp::resolver resolver(io_ctx_);
            auto endpoints = resolver.resolve(addr_.to_string(), std::to_string(port_));
            boost::asio::async_connect(sock_, endpoints,
           [this, self](std::error_code ec, const boost::asio::ip::tcp::endpoint& endpoint){
               if(!ec){
                   RootInfo() << "connected to " << endpoint.address().to_string() << ":" << endpoint.port();
                   // 开启异步读
                   do_async_read_header();
               }else{
                   RootError() << ec.message();
               }
           });
            std::thread client_net_thread([this](){
                io_ctx_.run();
            });
            client_net_thread.detach();
            RootInfo() << "client net is running in thread " << client_net_thread.get_id();
        }
        else{
            RootWarn() << "client net is running";
        }
    }

    void client::stop() {
        if(client_net_running()){
            io_ctx_.stop();
            client_net_running(false);
        }
        else{
            RootWarn() << "client net is not running, before stop it, you need start it first";
        }
    }

    void client::deliver(std::string message) {
        bool sending_message;
        common::message msg;
        if(msg.parse_from_string(message) < 0){
            return;
        }
        {
            std::unique_lock<std::mutex> lock(message_queue_mtx_);
            sending_message = !ordered_message_queue_.empty();
            ordered_message_queue_.push(msg);
        }
        if(!sending_message){
            do_async_send();
        }
    }

    void client::do_async_read_header() {
        boost::asio::async_read(sock_, boost::asio::buffer(received_message_.header(), common::message::header_size),
                            [this](std::error_code ec, std::size_t size){
           if(!ec && received_message_.parse_body_size() > 0){
               do_async_read_body();
           } else{
               // 断开连接
               RootInfo() << "closed connection with " << sock_.remote_endpoint().address().to_string() << ":" << sock_.remote_endpoint().port();
           }
        });
    }

    void client::do_async_read_body() {
        boost::asio::async_read(sock_, boost::asio::buffer(received_message_.body(), received_message_.size()),
                            [this](std::error_code ec, std::size_t size){
           if(!ec){
               int err = do_handle_received_data();
               if(err < 0){
                   // 断开连接
               }else{
                   received_message_.init();
                   do_async_read_header();
               }
           } else{
               // 断开连接
           }
        });
    }

    int client::do_handle_received_data() {
        RootDebug() << "received message: " << received_message_.body();
        // 可以在此进行反序列化response，然后调用用户回调处理响应
        return 0;
    }

    void client::do_async_send() {
        ordered_message_queue_.front().parse_body_size();
        RootInfo() << "sending message: " << ordered_message_queue_.front().body() << "/" << ordered_message_queue_.front().size();
        boost::asio::async_write(sock_, boost::asio::buffer(ordered_message_queue_.front().header(), ordered_message_queue_.front().size() + common::message::header_size),
                         [this](std::error_code ec, std::size_t size){
                             RootInfo() << "sent message: " << ordered_message_queue_.front().body() << "/" << ordered_message_queue_.front().size();
            bool can_continue_send;
            {
                std::unique_lock<std::mutex> lock(message_queue_mtx_);
                ordered_message_queue_.pop();
                can_continue_send = !ordered_message_queue_.empty();
            }
            if(!ec && can_continue_send){
                do_async_send();
            } else if(ec){
                RootError() << ec.message();
            }
        });
    }

    bool client::client_net_running() const{
        return client_net_running_;
    }

    void client::client_net_running(bool flag) {
        client_net_running_ = flag;
    }

} // lknet