//
// Created by zwj1 on 24-12-4.
//

/**
 * simple server
 * 此样例展示了如何通过 server 实例来实现自定义协议消息处理
 */

#include "Server.h"
#include "iostream"

void initLogger() {
    logger::logger::Root()->setLevel(packer::Debug);
    logger::logger::Root()->setLogFormater(
        "[%level] [%s {%Y-%m-%d %H-%M-%S}] [%path:%line]: %message\n");
}

enum PROTOCOL_MAPS {
    PROTOCOL_REQUEST_LOGIN = 0x101,
    PROTOCOL_REQUEST_REGISTER = 0x102,
    PROTOCOL_RESPONSE_STATUS = 0x201
};

/**
 * @note 自定义的DataBase结构体需要继承自DataBase抽象类
 *       并实现from_json和to_json两个接口
 */

// 用于登录的登录数据结构
struct LoginDataBase : public DataBase {
    std::string username;
    std::string password;

    void from_json(const nlohmann::json &j) override {
        j.at("username").get_to(username);
        j.at("password").get_to(password);
    }

    nlohmann::json to_json() override {
        return {{"username", username}, {"password", password}};
    }
};

// 用于注册的注册数据结构
struct RegisterDataBase : public DataBase {
    std::string username;
    std::string password;

    void from_json(const nlohmann::json &j) override {
        j.at("username").get_to(username);
        j.at("password").get_to(password);
    }

    nlohmann::json to_json() override {
        return {{"username", username}, {"password", password}};
    }
};

// 注册协议DataBase
void registerDataBase() {
    // 注册登录协议结构
    DataBaseRegister::getInstance().registerDataBase(
        PROTOCOL_REQUEST_LOGIN,
        []() { return std::make_unique<LoginDataBase>(); });
    // 注册注册协议结构
    DataBaseRegister::getInstance().registerDataBase(
        PROTOCOL_REQUEST_REGISTER,
        []() { return std::make_unique<RegisterDataBase>(); });
    // 注册响应状态结构体
    DataBaseRegister::getInstance().registerDataBase(
        PROTOCOL_RESPONSE_STATUS,
        []() { return std::make_unique<DataBaseStatus>(); });
};

// 注册协议Handler
void registerHandler() {
    RequestHandlerRouter::getInstance().registerHandlerGetterWithResponse(
        PROTOCOL_REQUEST_LOGIN, []() {
            return [](const Request &r) -> Response {
                std::cout << "this is handler(id: 1)" << std::endl;
                return {
                    PROTOCOL_RESPONSE_STATUS,
                    std::make_unique<DataBaseStatus>(DataBaseStatus::Success)};
            };
        });
    RequestHandlerRouter::getInstance().registerHandlerGetterWithResponse(
        PROTOCOL_REQUEST_REGISTER,
        []() -> RequestHandlerRouter::HandlerWithResponse {
            return [](const Request &r) -> Response {
                std::cout << "this is handler(id: 2)" << std::endl;
                // 返回响应
                return {
                    PROTOCOL_RESPONSE_STATUS,
                    std::make_unique<DataBaseStatus>(DataBaseStatus::Success)};
            };
        });
}

void initServer() {
    initLogger();
    registerDataBase();
    registerHandler();
}

int main(int argc, char *argv[]) {
    initServer();
    // connect to 127.0.0.1 : 8080
    std::string host;
    unsigned short port = 0;
    if (argc == 1) {
        host = "127.0.0.1";
        port = 18080;
    } else if (argc == 2) {
        host = argv[1];
        port = 18080;
    } else if (argc == 3) {
        host = argv[1];
        port = std::stoi(argv[2]);
        if (!port) {
            RootError() << "argument 'port' parse failed.";
            exit(EXIT_FAILURE);
        }
    } else {
        RootError() << "usage: " << argv[0] << " [host] [port]";
    }

    Server s(host, port);
    s.start();

    return 0;
}