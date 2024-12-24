//
// Created by zwj1 on 24-12-24.
//

#include "ConfigLoader.h"
#include "iostream"
#include "fstream"

using namespace lknet::util;

int main(int argc, char* argv[]) {
    std::string filePath = "config.json";
    std::ofstream file(filePath);
    try{
        // 向file写入json格式配置（默认）
        if(!file.is_open()){
            throw std::runtime_error("file: " + filePath = " can not be opened");
        }
        file << R"({"hostname": "127.0.0.1", "port": "7890", "database": "myTestDatabase"})";
        file.flush();
        file.close();

        // 读取文件内容，并解析指ConfigLoader单例
        auto& configLoader = ConfigLoader::getInstance();
        configLoader.loadConfig(filePath);
        auto& configMap = configLoader.getConfig();

        // 打印配置内容
        for(auto& kv: configMap){
            std::cout << kv.first << ": " << kv.second << std::endl;
        }
    }catch (std::exception& e){
        std::cout << e.what() << std::endl;
    }
}