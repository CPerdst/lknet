## 基于Boost.asio的简单异步小框架

客户端与服务器内置消息回调，可以通过设置对应的回调来处理接受到的消息

## 简单使用

### 拉取代码

git clone --recursive https://github.com/CPerdst/lknet.git

### 编译(建议使用 GNU 编译套件)

``` shell
cmake -S path/to/lknet -B path/to/lknet/build
cmake --build path/to/lknet/build --target lknet
```

注：可以使用 cmake --build path/to/lknet/build --target help 获取帮助构建信息
