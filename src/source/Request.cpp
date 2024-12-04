//
// Created by zwj1 on 24-12-4.
//

#include "Request.h"

Request::RequestFactory Request::RequestFactory::instance;

Request::RequestFactory& Request::RequestFactory::getInstance() {
    return instance;
}

void Request::RequestFactory::registerDatabase(unsigned short protocolId_, Request::RequestFactory::Creator c) {
    creators_map[protocolId_] = std::move(c);
}

std::unique_ptr<DataBase> Request::RequestFactory::create(unsigned short protocolId_) const {
    auto it = creators_map.find(protocolId_);
    if(it != creators_map.end()){
        return it->second();
    }
    throw std::runtime_error("Unknown protocolId " + std::to_string(protocolId_) + " at complied time");
}

nlohmann::json Request::to_json() const {
    return {{"protocol_id", protocolId}, {"data", data->to_json()}};
}

void Request::from_json(nlohmann::json &j) {
    protocolId = j.at("protocol_id");
    data = RequestFactory::getInstance().create(protocolId);
    data->from_json(j.at("data"));
}

unsigned short Request::getProtocolId() const {
    return protocolId;
}

std::unique_ptr<DataBase>& Request::getData() {
    return data;
}

void Request::setProtocolId(unsigned short id) {
    protocolId = id;
}

void Request::setData(std::unique_ptr<DataBase> data_) {
    data = std::move(data_);
}