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