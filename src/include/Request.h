//
// Created by zwj1 on 24-12-4.
//

#ifndef LKNET_REQUEST_H
#define LKNET_REQUEST_H

#include "common.h"
#include "functional"
#include "memory"
#include "map"

class Request {
public:

    class RequestFactory{
    public:
        using Creator = std::function<std::unique_ptr<DataBase>()>;

        RequestFactory(const RequestFactory&) = delete;
        RequestFactory& operator=(const RequestFactory&) = delete;

        void registerDatabase(unsigned short protocolId_, Creator c);

        std::unique_ptr<DataBase> create(unsigned short protocolId_) const;

        static RequestFactory& getInstance();

    private:
        RequestFactory() = default;
        ~RequestFactory() = default;

        std::map<int, Creator> creators_map;
        static RequestFactory instance;
    };

    Request() = default;
    ~Request() = default;

    nlohmann::json to_json() const;

    void from_json(nlohmann::json& j);

    unsigned short getProtocolId() const;

    std::unique_ptr<DataBase>& getData();

    void setProtocolId(unsigned short id);

    void setData(std::unique_ptr<DataBase> data_);

private:
    unsigned short protocolId{};
    std::unique_ptr<DataBase> data;
};

#endif //LKNET_REQUEST_H
