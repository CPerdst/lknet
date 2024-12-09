// iobase.cpp

#include "IOBase.h"
#include <iostream>

IOBase::IOBase(boost::asio::io_context& io_context):
sock(io_context),
messageHandler(nullptr),
closeHandler(nullptr),
isIOBaseRunning(false)
{

}

IOBase::IOBase(boost::asio::ip::tcp::socket& socket):
sock(std::move(socket)),
messageHandler(nullptr),
closeHandler(nullptr),
isIOBaseRunning(false)
{

}

void IOBase::start() {
    doReadHeader();
    isIOBaseRunning = true;
}

void IOBase::send(const Message &msg) {
    bool canDoWrite = false;
    {
        std::unique_lock<std::mutex> lock(writeMutex);
        canDoWrite = writeMessages.empty();
        writeMessages.push_back(msg);
    }
    if(canDoWrite){
        doWrite();
    }
}

void IOBase::setMessageHandler(const std::function<void(Message, IOBase*)> &handler) {
    messageHandler = handler;
}

void IOBase::setCloseHandler(const std::function<void(void)> &handler) {
    closeHandler = handler;
}

boost::asio::ip::tcp::socket &IOBase::socket() {
    return sock;
}

void IOBase::doReadHeader() {
    auto self(shared_from_this());

    char* headBuffer = readMessage.headBuffer();
    boost::asio::async_read(sock, boost::asio::buffer(headBuffer, Message::MESSAGE_HEADER_SIZE),
                            [this, self](std::error_code ec, std::size_t size){
        readMessage.parseMessageHeader();
        if(!ec && readMessage.isValidMessage()){
            doReadBody();
        }else{
            RootError() << "socket closed as head receive failed";
            if(closeHandler) closeHandler();
        }
    });
}

void IOBase::doReadBody() {
    auto self(shared_from_this());

    int readMessageSize = readMessage.messageBodySize;
    char* body = readMessage.bodyBuffer();
    boost::asio::async_read(sock, boost::asio::buffer(body, readMessageSize),
                            [this, self](std::error_code ec, std::size_t size){
        try{
            if(!ec){
                if(messageHandler){
                    messageHandler(readMessage, this);
                }
                doReadHeader();
            }else{
                throw std::runtime_error("socket closed as body receive failed");
            }
        }catch(std::exception& e){
            RootError() << ": closed connection with: " \
            << sock.remote_endpoint().address().to_string() << ":" << sock.remote_endpoint().port() \
            << " as: " << e.what();
            if(closeHandler) closeHandler();
        }
    });
}

void IOBase::doWrite() {
    auto self(shared_from_this());

    while(!isIOBaseRunning);

    std::unique_lock<std::mutex> lock(writeMutex);
    char* buffer = writeMessages.front().headBuffer();
    int bufferSize = writeMessages.front().messageBodySize + Message::MESSAGE_HEADER_SIZE;
    boost::asio::async_write(sock, boost::asio::buffer(buffer, bufferSize),
                             [this, self](std::error_code ec, std::size_t size){
        if(!ec){
            bool canWrite = false;
            {
                std::unique_lock<std::mutex> lock(writeMutex);
                writeMessages.pop_front();
                if(!writeMessages.empty()){
                    canWrite = true;
                }
            }
            if(canWrite)
                doWrite();
        }else{
            RootError() << "socket closed as for write message failed: " << ec.message() <<std::endl;
            if(closeHandler)
                closeHandler();
        }
    });
}