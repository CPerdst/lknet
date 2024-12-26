//
// Created by l1Akr on 2024/12/5.
//

#include <iostream>

#include "common.h"

// enum DATABASE_MAPS{
//     PERSON = 1,
//     ANIMAL = 2
// };
//
// struct person: DataBase {
//     int age{};
//     std::string name;
//
//     person() = default;
//     person(int age, std::string name):
//     age(age),
//     name(name){}
//
//     int getAge() {
//         return age;
//     }
//
//     std::string getName() {
//         return name;
//     }
//
//     void from_json(nlohmann::json& j) override {
//         age = j.at("age");
//         name = j.at("name");
//     }
//
//     nlohmann::json to_json() override {
//         return {{"age", age}, {"name", name}};
//     }
// };
//
// struct animal: DataBase {
//     bool sex{};
//     std::string name;
//
//     animal() = default;
//     animal(bool sex, std::string name):
//     sex(sex),
//     name(name){}
//
//     void from_json(nlohmann::json& j) override {
//         sex = j.at("sex");
//         name = j.at("name");
//     }
//
//     nlohmann::json to_json() override {
//         return {{"sex", sex}, {"name", name}};
//     }
// };

#include "variant"

int main() {
    /*
  //    std::unique_ptr<DataBase> obj = std::make_unique<person>(19, "John");
  //    std::cout << obj->to_json().dump(4) << std::endl;
  //    std::unique_ptr<DataBase> obj2 = std::make_unique<animal>(true, "John");
  //    std::cout << obj2->to_json().dump(4) << std::endl;
  //    // =============== Request Test ===============
  //    DataBaseRegister::getInstance().registerDataBase(1, [](){
  //        return std::make_unique<person>();
  //    });
  //    Request r(1, std::move(std::make_unique<person>(19, "John")));
  //    nlohmann::json j = r.to_json();
  //    std::cout << j.dump(4) << std::endl;
  //
  //    Request r_receive;
  //    try{
  //        r_receive.from_json(j);
  //    }catch (std::exception& e){
  //        std::cout << e.what() << std::endl;
  //        exit(EXIT_FAILURE);
  //    }
  //    switch (r_receive.id) {
  //        case PERSON:
  //        {
  //            std::unique_ptr<person>
  per(dynamic_cast<person*>(r_receive.data.release()));
  //            std::cout << "this is a person:\n" << "age: " << per->getAge()\
  //            << "\nname: " <<per->getName() << std::endl;
  //            break;
  //        }
  //        default:
  //        {
  //            std::cout << "error" << std::endl;
  //            exit(EXIT_FAILURE);
  //        }
  //    }
    */

    std::variant<int, std::string> item;

    return 0;
}