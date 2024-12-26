//
// Created by zwj1 on 24-12-4.
//

#ifndef LKNET_COMMON_H
#define LKNET_COMMON_H

#include "json.hpp"
#include "mutex"
#include "tools.h"

struct DLL_API DataBase {
    DataBase() = default;
    virtual ~DataBase() = default;

    virtual nlohmann::json to_json() = 0;  // 序列化

    virtual void from_json(const nlohmann::json &j) = 0;  // 反序列化
};

/**
 * 用于测试的 “测试数据类型”
 */

struct DLL_API myTestDataBase : public DataBase {
    int age;
    std::string name;

    nlohmann::json to_json() override;  // 序列化

    void from_json(const nlohmann::json &j) override;  // 反序列化
};

struct DLL_API DataBaseStatus : public DataBase {
    enum RESPONSE_STATUS { Success = 200 };

    RESPONSE_STATUS status;

    DataBaseStatus() = default;

    ~DataBaseStatus() override = default;

    explicit DataBaseStatus(RESPONSE_STATUS status);

    nlohmann::json to_json() override;  // 序列化

    void from_json(const nlohmann::json &j) override;  // 反序列化
};

/**
 * 单例模式 DataBaseRegister
 */

class DLL_API DataBaseRegister {
   public:
    DataBaseRegister(const DataBaseRegister &) = delete;
    DataBaseRegister &operator=(const DataBaseRegister &) = delete;

    using Creator = std::function<std::unique_ptr<DataBase>()>;

    static DataBaseRegister &getInstance();

    void registerDataBase(unsigned short id, const Creator &c);

    /**
     * 根据注册的 id-creator 创建相应的 std::unique_ptr<DataBase> 对象。
     * throw std::runtime_error();
     *
     * @param id
     * @return std::unique_ptr<DataBase>
     */
    std::unique_ptr<DataBase> create(unsigned short id);

   private:
    DataBaseRegister() = default;
    ~DataBaseRegister() = default;

    static DataBaseRegister instance;

    std::mutex instanceMutex;

    std::map<unsigned short, Creator> creatorsMap{};
};

class DLL_API Request {
   public:
    Request() = default;
    ~Request() = default;

    Request(unsigned short id, std::unique_ptr<DataBase> data);

    nlohmann::json to_json() const;

    void from_json(const nlohmann::json &j);

    unsigned short getId() const;

    const std::unique_ptr<DataBase> &getData() const;

    void setId(unsigned short id);

    void setData(std::unique_ptr<DataBase> data_);

   private:
    unsigned short id{};
    std::unique_ptr<DataBase> data;
};

class DLL_API Response {
   public:
    Response() = default;
    ~Response() = default;

    Response(unsigned short id, std::unique_ptr<DataBase> data);

    nlohmann::json to_json() const;

    void from_json(const nlohmann::json &j);

    unsigned short getId() const;

    const std::unique_ptr<DataBase> &getData() const;

    void setId(unsigned short id);

    void setData(std::unique_ptr<DataBase> data_);

   private:
    unsigned short id{};
    std::unique_ptr<DataBase> data;
};

// class DLL_API ProtocolRegister {
// public:
//
//     class IProtocolPayload{
//     public:
//         IProtocolPayload() = default;
//         virtual ~IProtocolPayload() = default;
//
//         virtual unsigned short getProtocol() const = 0;
//         virtual std::unique_ptr<INlohmannDataBase> getData() = 0;
//     };
//
//     class RequestImpl: public IProtocolPayload {
//     public:
//         RequestImpl() = default;
//         ~RequestImpl() override = default;
//
//         unsigned short getProtocol() const override;
//
//         std::unique_ptr<INlohmannDataBase> getData() override;
//
//     private:
//         unsigned short protocol; // 协议号
//         std::unique_ptr<INlohmannDataBase> data; // 实际数据
//     };
//
//     class Response {
//
//     };
//
//     using Creator = std::function<std::unique_ptr<DataBase>()>;
//
//     ProtocolRegister(const ProtocolRegister&) = delete;
//     ProtocolRegister& operator=(const ProtocolRegister&) = delete;
//
//     static std::unique_ptr<ProtocolRegister>& getInstance();
//
//
//
// private:
//     ProtocolRegister() = default;
//     ~ProtocolRegister() = default;
//
//     // 懒汉模式
//     static std::unique_ptr<ProtocolRegister> instance;
//
//     // 锁保证instance的使用
//     std::mutex instanceMutex;
//
//     // 用于存储实际协议与Creator的映射
//     std::map<unsigned short, INlohmannDataBase> creatorsMap;
// };

#endif  // LKNET_COMMON_H
