//
// Created by zwj1 on 24-12-24.
//

#include "ConfigLoader.h"
#include "iostream"

using namespace lknet::util;

int main(int argc, char* argv[]) {
    std::string filePath = "config.json";
    try{
        ConfigLoader::getInstance()->loadConfigFromJsonFile(filePath);
        auto& configMap = ConfigLoader::getInstance()->getConfig();
        for(auto& kv: configMap){
            std::cout << kv.first << ": " << kv.second << std::endl;
        }
    }catch (std::exception& e){
        std::cout << e.what() << std::endl;
    }
}