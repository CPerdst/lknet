//
// Created by zwj1 on 24-12-4.
//

#include "Request.h"
#include "eventCapturer.h"

struct service: public DataBase{
    std::string serviceName;
    int value{};

    service() = default;
    ~service() = default;
    service(int val, std::string name): value(val), serviceName(name) {}

    nlohmann::json to_json() override{
        return {{"service_name", serviceName}, {"value", value}};
    }

    void from_json(nlohmann::json &j) override{
        j.at("service_name").get_to(serviceName);
        j.at("value").get_to(value);
    }
};

int main(){
    Request::RequestFactory::Creator asd = [](){
        return std::make_unique<myTestDataBase>();
    };
    // 注册 myTestDataBase
    RootInfo() << "----------- First register data struct \"myTestDataBase\" -----------" << std::endl;
    Request::RequestFactory::getInstance().registerDatabase(0, [](){
        return std::make_unique<myTestDataBase>();
    });

    // 反序列化测试
    RootInfo() << "----------- test serialization -----------" << std::endl;

    Request test{};

    test.setProtocolId(0);
    std::unique_ptr<myTestDataBase> testData = std::make_unique<myTestDataBase>();
    testData->name = "zhang_kun";
    testData->age = 19;
    test.setData(std::move(testData));

    nlohmann::json j = test.to_json();
    RootInfo() << "\nserialization str: \n" << j.dump(4) << std::endl;

    // 序列化测试

    RootInfo() << "----------- test deserialization -----------" << std::endl;

    Request test2{};
    test2.from_json(j);
    RootInfo() << "\nprotocol_id: " << test2.getProtocolId() << std::endl;
    RootInfo() << "\ndata: \n" << test2.getData()->to_json().dump(4) << std::endl;

    RootInfo() << "----------- Second register data struct \"service\" -----------" << std::endl;
    Request::RequestFactory::getInstance().registerDatabase(1, [](){
        return std::make_unique<service>();
    });

    RootInfo() << "----------- test serialization -----------" << std::endl;

    Request test3{};
    test3.setProtocolId(1);
    test3.setData(std::make_unique<service>(1, "hello_world_service"));

    nlohmann::json j2 = test3.to_json();
    RootInfo() << "\nserialization str: \n" << j2.dump(4) << std::endl;

    RootInfo() << "----------- test deserialization -----------" << std::endl;

    Request test4{};
    test4.from_json(j2);

    RootInfo() << "\nprotocol_id: " << test4.getProtocolId() << std::endl;
    RootInfo() << "\ndata:\n" << test4.getData()->to_json().dump(4) << std::endl;

}

