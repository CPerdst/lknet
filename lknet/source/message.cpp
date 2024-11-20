//
// Created by zwj1 on 2024/11/13.
//

#include "message.h"
#include "iostream"

namespace lknet::common {
    char *message::body() {
        return header() + header_size;
    }

    int message::size() const {
        return body_size_;
    }

    char *message::header() {
        return data;
    }

    int message::parse_body_size() {
        std::string h(header(), header_size);
        long body_size;
        char* tmp;
        body_size = std::strtol(h.c_str(), &tmp, 10);
        if(body_size > 0 && body_size <= max_body_size){
            body_size_ = static_cast<int>(body_size);
            return body_size_;
        }
        return -1;
    }

    int message::parse_from_string(std::string &message) {
        int message_size = static_cast<int>(message.size());
        // 解析长度到size
        if(message.empty() || message_size > max_body_size){
            return -1;
        }
        auto size_s = std::to_string(message_size);
        if(size_s.size() > header_size){
            return -1;
        }
        strncpy(header(), size_s.c_str(), size_s.size());
        strncpy(body(), message.c_str(), message_size);
        return 0;
    }

    void message::init() {
        memset(header(), 0, sizeof(char) * (header_size + max_body_size));
        body_size_ = 0;
    }
} // lknet
// common