//
// Created by zwj1 on 24-12-4.
//

#ifndef LKNET_COMMON_H
#define LKNET_COMMON_H

#include "json.hpp"
#include "tools.h"

struct DLL_API DataBase {
    DataBase() = default;
    ~DataBase() = default;

    virtual nlohmann::json to_json() = 0;
    virtual void from_json(nlohmann::json& j) = 0;
};

struct DLL_API myTestDataBase: public DataBase{
    int age;
    std::string name;

    nlohmann::json to_json() override;
    void from_json(nlohmann::json &j) override;
};

#endif //LKNET_COMMON_H
