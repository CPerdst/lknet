//
// Created by zwj1 on 24-12-24.
//

#ifndef LKNET_CONFIGLOADER_H
#define LKNET_CONFIGLOADER_H

#include "json.hpp"
#include "map"
#include "memory"
#include "mutex"

namespace lknet{
    namespace util {
        class ConfigLoader {
        public:
            static std::shared_ptr<lknet::util::ConfigLoader> getInstance();
            virtual void loadConfigFromJsonFile(std::string path);
            virtual std::map<std::string, std::string>& getConfig();

        protected:
            std::map<std::string, std::string> configMap;

        private:
            static std::shared_ptr<ConfigLoader> instance;
        };

        class ConfigLoaderJsonCore: public ConfigLoader {
        public:
            void loadConfigFromJsonFile(std::string path) override;
        };
    }
}





#endif //LKNET_CONFIGLOADER_H
