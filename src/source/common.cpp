//
// Created by zwj1 on 24-12-4.
//

#include "common.h"

void myTestDataBase::from_json(nlohmann::json &j) {
    j.at("age").get_to(age);
    j.at("name").get_to(name);
}

nlohmann::json myTestDataBase::to_json() {
    return {{"age", age}, {"name", name}};
}

// ----------------------
// DataBaseRegister 实现
// ----------------------

DataBaseRegister DataBaseRegister::instance;

DataBaseRegister& DataBaseRegister::getInstance() {
    return instance;
}

void DataBaseRegister::registerDataBase(unsigned short id, const Creator& c){
    std::unique_lock<std::mutex> lock(instanceMutex);
    creatorsMap.insert({id, c});
}

std::unique_ptr<DataBase> DataBaseRegister::create(unsigned short id) {
    // 防止多线程同时访问creatorsMap造成问题，需要枷锁
    std::unique_lock<std::mutex> lock(instanceMutex);
    auto it = creatorsMap.find(id);
    if(it != creatorsMap.end()) {
        return it->second();
    }
    throw std::runtime_error("can not find DataBase type(id:" + std::to_string(id) + ")");
}

// ----------------------
// DataBaseStatus 实现
// ----------------------

DataBaseStatus::DataBaseStatus(RESPONSE_STATUS status):
status(status)
{

}

nlohmann::json DataBaseStatus::to_json() {
    return {"status", status};
}

void DataBaseStatus::from_json(nlohmann::json &j) {
    status = j.at("status");
}

// ----------------------
// Request 实现
// ----------------------

Request::Request(unsigned short id, std::unique_ptr<DataBase> data):
id(id),
data(std::move(data))
{

}

nlohmann::json Request::to_json() const {
    return {{"id", id}, {"data", data->to_json()}};
}

void Request::from_json(nlohmann::json &j) {
    id = j.at("id");
    data = DataBaseRegister::getInstance().create(id);
    data->from_json(j.at("data"));
}

unsigned short Request::getId() const {
    return id;
}

const std::unique_ptr<DataBase>& Request::getData() const {
    return data;
}

void Request::setId(unsigned short id_) {
    id = id_;
}

void Request::setData(std::unique_ptr<DataBase> data_) {
    data = std::move(data_);
}

// ----------------------
// Response 实现
// ----------------------

Response::Response(unsigned short id, std::unique_ptr<DataBase> data):
id(id),
data(std::move(data))
{

}

nlohmann::json Response::to_json() const {
    return {{"id", id}, {"data", data->to_json()}};
}

void Response::from_json(nlohmann::json &j) {
    id = j.at("id");
    data = DataBaseRegister::getInstance().create(id);
    data->from_json(j.at("data"));
}

unsigned short Response::getId() const {
    return id;
}

const std::unique_ptr<DataBase>& Response::getData() const {
    return data;
}

void Response::setId(unsigned short id_) {
    id = id_;
}

void Response::setData(std::unique_ptr<DataBase> data_) {
    data = std::move(data_);
}

