//
// Created by zwj1 on 2024/11/12.
//

#ifndef LKNET_LKSEREVR_H
#define LKNET_LKSEREVR_H

namespace lknet {

    /**
     * @brief lkserver是面向用户的服务器接口
     */
    class lkserver {
    public:
        lkserver() = default;
        virtual ~lkserver() = default;
        /**
         * @brief 运行服务器（需要先初始化服务器的必要资源：地址端口）
         */
        virtual void run() = 0;
    };

} // lknet

#endif //LKNET_LKSEREVR_H
