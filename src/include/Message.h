// message.h

#ifndef MESSAGE_H
#define MESSAGE_H

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <string>

class IOBase;

class IMessage{
public:
    IMessage() = default;
    virtual ~IMessage() = default;

    virtual bool buildMessageFromString(const std::string &message) = 0;

    virtual bool buildMessageFromArray(const char* buffer, int size) = 0;

    virtual std::string parseMessageToString() = 0;

    virtual bool isValidMessage() = 0;

    virtual void parseMessageHeader() = 0;
};

class Message: public IMessage{
public:
    enum {MESSAGE_HEADER_SIZE = 4};
    enum {MESSAGE_MAX_BODY_SIZE = 512};

    friend IOBase;

    Message() = default;
    ~Message() override = default;

    std::string parseMessageToString() override{
        if(!isValidMessage()){
            return "";
        }
        std::string tmp(data + MESSAGE_HEADER_SIZE, messageBodySize);
        return tmp;
    }

    bool buildMessageFromString(const std::string &message) override{
        char* head = headBuffer();
        char* body = bodyBuffer();
        int messageSize = message.size();
        if(!isValidSize(messageSize)){
            return false;
        }
        // 将size写入头部
        std::string tmpHeadString = std::to_string(messageSize);
        std::strncpy(head, tmpHeadString.c_str(), tmpHeadString.size());
        head[MESSAGE_HEADER_SIZE - 1] = '\0';
        messageBodySize = messageSize;

        // 将buffer中的数据拷贝到body部分
        std::strncpy(body, message.c_str(), messageSize);
        return true;
    }

    bool isValidMessage() override{
        char* head = headBuffer();
        char* body = bodyBuffer();
        if(head[3] != '\0' || !isValidSize(messageBodySize)){
            return false;
        }
        return true;
    }

    void parseMessageHeader() override{
        int tmpBodySize = std::atoi(headBuffer());
        messageBodySize = tmpBodySize;
    }

    bool buildMessageFromArray(const char *buffer, int size) override{
        char* head = headBuffer();
        char* body = bodyBuffer();
        if(!isValidSize(size)){
            return false;
        }
        // 将size写入头部
        std::string tmpHeadString = std::to_string(size);
        std::strncpy(head, tmpHeadString.c_str(), tmpHeadString.size());
        head[MESSAGE_HEADER_SIZE - 1] = '\0';
        messageBodySize = size;

        // 将buffer中的数据拷贝到body部分
        std::strncpy(body, buffer, size);
        return true;
    }

    [[nodiscard]] int getMessageBodySize() const {
        return messageBodySize;
    }

    [[nodiscard]] const char* getMessageBody() const {
        return data + MESSAGE_HEADER_SIZE;
    }

protected:
    char* headBuffer() {
        return data;
    };
    char* bodyBuffer() {
        return data + MESSAGE_HEADER_SIZE;
    };

private:
    static bool isValidSize(int size) {
        return (size > 0) && (size <= MESSAGE_MAX_BODY_SIZE);
    }

    char data[MESSAGE_HEADER_SIZE + MESSAGE_MAX_BODY_SIZE]{};
    int messageBodySize{};

};

#endif // MESSAGE_H