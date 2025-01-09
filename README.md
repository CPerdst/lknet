## 基于Boost.asio的简单异步小框架

客户端与服务器内置消息回调，可以通过设置对应的回调来处理接受到的消息

## 功能介绍

lknet是一款基于Boost.Asio开发的异步网络框架，lknet 致力于设计成为一个轻便型、接口友好、易于拓展以及使用的 C++ 网络模型库。

### 客户端模块

``` C++
//
// Created by zwj1 on 24-12-4.
//

/**
 * 代码使用 Client 类创建客户端对象。
 * start 函数开启客户端对象，send 函数-
 * 可以发送数据到已连接的服务器。
 */

#include "iostream"
#include "Client.h"

void initLogger() {
    logger::logger::Root()->setLevel(packer::Debug);
    logger::logger::Root()->setLogFormater("[%level] [%s {%Y-%m-%d %H-%M-%S}]: %message\n");
}

int main(int argc, char* argv[]) {
    initLogger();
    // connect to 127.0.0.1 : 8080
    std::string host;
    unsigned short port = 0;
    if(argc == 1){
        host = "127.0.0.1";
        port = 8080;
    }else if(argc == 2){
        host = argv[1];
        port = 8080;
    }else if(argc == 3){
        host = argv[1];
        port = std::stoi(argv[2]);
        if(!port){
            RootError() << "argument 'port' parse failed.";
            exit(EXIT_FAILURE);
        }
    }else{
        RootError() << "usage: " << argv[0] << " [host] [port]";
    }
    Client c(host, port);
    // 让 Client 运行在新线程之中，需要保证主线程一直运行
    c.start(true);
    while(true){
        c.send("hello server");
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
```

客户端接口介绍：

（待完善）

-------

### 服务端模块

```c++
//
// Created by zwj1 on 24-12-4.
//

/**
 * 代码使用 Server 类创建了服务器对象，通过 start 函数，
 * 启动该对象。
 * server 对象能够自动接收来自客户端的数据，并将其使用-
 * lklog 模块进行输出。
 */

#include "iostream"
#include "Server.h"

void initLogger() {
    logger::logger::Root()->setLevel(packer::Debug);
    logger::logger::Root()->setLogFormater("[%level] [%s {%Y-%m-%d %H-%M-%S}]: %message\n");
}

int main(int argc, char* argv[]) {
    initLogger();
    // connect to 127.0.0.1 : 8080
    std::string host;
    unsigned short port = 0;
    if(argc == 1){
        host = "127.0.0.1";
        port = 8080;
    }else if(argc == 2){
        host = argv[1];
        port = 8080;
    }else if(argc == 3){
        host = argv[1];
        port = std::stoi(argv[2]);
        if(!port){
            RootError() << "argument 'port' parse failed.";
            exit(EXIT_FAILURE);
        }
    }else{
        RootError() << "usage: " << argv[0] << " [host] [port]";
    }

    Server s(host, port);
    s.start();

    return 0;
}
```

服务端模块接口介绍：

（待完善）

----

### 其他功能

（待完善）

---------

## 简单使用

### 拉取代码

git clone --recursive https://github.com/CPerdst/lknet.git

### 编译(建议使用 GNU 编译套件)

项目仅支持C++17及以上，后面会考虑适配C++11

#### Windows下自动编译脚本

使用build.bat脚本

1、指定BoostConfig.cmake所在的路径

2、自动更新lklog子模块

3、选择编译模式（Debug、Release

4、选择CRT输出模式（静态、动态

5、选择本机支持的编译套件（v143-x64、v143-32、v142-x64、v142-32

6、选择是否编译使用样例与测试样例

#### 手动编译

``` shell
cmake -S path/to/lknet -B path/to/lknet/build
cmake --build path/to/lknet/build --target lknet
```

cmake编译选项

输出静态链接库(默认)

-DCRT_BUILD_TYPE=static 

输出动态链接库

-DCRT_BUILT_TYPE=dynamic

项目依赖于Boost库，需要为项目指定BoostConfig.cmake所在的路径

-DUSER_PRESET_BOOST_DIR=/path/to/BoostConfig.cmake

可以指定是否编译样例与测试样例

-DEXAMPLE_BUILD

-DTEST_BUILD

注：可以使用 cmake --build path/to/lknet/build --target help 获取帮助构建信息

注：如果使用 windows 系统进行构建，需要注意 boost 库的路径，可以选择将 boost 库路径设置为 Cmake 可识别的环境变量。