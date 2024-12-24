//
// Created by zwj1 on 24-12-24.
//

#include "ConfigLoader.h"

#include <memory>
#include "fstream"
#include "eventCapturer.h"

using namespace lknet::util;

std::shared_ptr<ConfigLoader> ConfigLoader::instance = nullptr;

std::shared_ptr<ConfigLoader> ConfigLoader::getInstance() {
    if(!instance){
        instance = std::make_shared<ConfigLoaderJsonCore>();
    }
    return instance;
}

void ConfigLoader::loadConfigFromJsonFile(std::string) {}

std::map<std::string, std::string> &ConfigLoader::getConfig() {
    return configMap;
}

void ConfigLoaderJsonCore::loadConfigFromJsonFile(std::string path) {
    // 加载json文件，并将其解析成configMap
    try{
        nlohmann::json configJson;
        std::ifstream file(path);
        if(!file.is_open()){
            throw std::runtime_error("file: " + path + " can not be opened.");
        }
        file >> configJson;
        for(auto &kv: configJson.items()){
            if((!kv.value().is_string()) && (!kv.value().is_number())){
                throw std::runtime_error("value must be string");
            }
            configMap[kv.key()] = to_string(kv.value());
        }
        RootInfo() << "Successfully Loaded ConfigFile: " << path << std::endl;
    }catch (std::exception& e){
        throw e;
    }
}

