//
// Created by zwj1 on 24-12-24.
//

#ifndef LKNET_CONFIGLOADER_H
#define LKNET_CONFIGLOADER_H

#include "tools.h"
#include "json.hpp"
#include "map"
#include "memory"
#include "mutex"

namespace lknet::util {
    class DLL_API ConfigLoader {
    public:
        using ConfigOptions = std::map<std::string, std::string>;

        static ConfigLoader &getInstance();

        void loadConfig(const std::string&);

        void setConfigOptions(const ConfigOptions&);

        void setConfigOptions(ConfigOptions&&);

        virtual std::map<std::string, std::string>& getConfig();

    protected:
        std::map<std::string, std::string> configMap;

        /**
         * "parseFormat": default for json, also has other options: "xml"
         */
        ConfigOptions optionMap;

    private:
        ConfigLoader() = default;
        ~ConfigLoader() = default;

        static ConfigLoader instance;
    };

    namespace tools {
        class DLL_API ConfigLoaderJsonCore {
        public:
            ConfigLoaderJsonCore() = default;
            ~ConfigLoaderJsonCore() = default;

            ConfigLoaderJsonCore(const std::string&, std::map<std::string, std::string>&);

        private:
            void parseJsonFileToMap(const std::string&, std::map<std::string, std::string>&);
        };
    }
}

#endif //LKNET_CONFIGLOADER_H
