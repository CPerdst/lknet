//
// Created by zwj1 on 2024/11/13.
//

#ifndef LKNET_MESSAGE_H
#define LKNET_MESSAGE_H

#include "iostream"

namespace lknet {
    namespace common {

        class message {
        public:
            static constexpr unsigned int header_size = 4;
            static constexpr unsigned int max_body_size = 512;
            message() = default;
            ~message() = default;

            char* body();
            char* header();
            int size() const;
            int parse_body_size();
            int parse_from_string(std::string& message);
            void init();
        private:
            char header_[header_size]{};
            char body_[max_body_size]{};
            char data[header_size + max_body_size]{};
            int body_size_ = 0;
        };

    } // lknet
} // common

#endif //LKNET_MESSAGE_H
