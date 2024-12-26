//
// Created by zwj1 on 24-12-24.
//

#include "ConfigLoader.h"

#include <memory>

#include "eventCapturer.h"
#include "fstream"

using namespace lknet::util;
using namespace lknet::util::tools;

// ------------------------
// ConfigLoader 实现
// ------------------------

ConfigLoader ConfigLoader::instance;

ConfigLoader &ConfigLoader::getInstance() { return instance; }

void ConfigLoader::loadConfig(const std::string &path) {
    if (path.empty()) {
        throw std::runtime_error("path is empty");
    }
    auto it = optionMap.find("parseFormat");
    if (it == optionMap.end() || it->second == "json") {
        ConfigLoaderJsonCore configParser(path, configMap);
    } else if (it->second == "xml") {
        throw std::runtime_error("parseFormat(xml) no implementation");
    } else {
        throw std::runtime_error("please figure parseFormat in " + path);
    }
}

void ConfigLoader::setConfigOptions(
    lknet::util::ConfigLoader::ConfigOptions &&options) {
    if (!options.empty()) {
        optionMap = std::move(options);
    }
}

void ConfigLoader::setConfigOptions(
    const lknet::util::ConfigLoader::ConfigOptions &options) {
    if (!options.empty()) {
        optionMap = options;
    }
}

ConfigLoader::ConfigOptions &ConfigLoader::getConfig() { return configMap; }

// ------------------------
// ConfigLoaderJsonCore 实现
// ------------------------

ConfigLoaderJsonCore::ConfigLoaderJsonCore(
    const std::string &path, std::map<std::string, std::string> &map) {
    parseJsonFileToMap(path, map);
}

void ConfigLoaderJsonCore::parseJsonFileToMap(
    const std::string &path, std::map<std::string, std::string> &configMap) {
    // 加载json文件，并将其解析成configMap
    nlohmann::json configJson;
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("file: " + path + " can not be opened.");
    }
    file >> configJson;
    for (auto &kv : configJson.items()) {
        if ((!kv.value().is_string()) && (!kv.value().is_number())) {
            throw std::runtime_error("value must be string");
        }
        configMap[kv.key()] = to_string(kv.value());
    }
    RootInfo() << "Successfully Loaded ConfigFile: " << path << std::endl;
}
