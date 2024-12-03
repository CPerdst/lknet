//
// Created by l1Akr on 2024/12/3.
//

#include "tools.h"
#include "iostream"

template <typename name>
void printClass(){
    using socket_t = name;
    std::cout << std::boolalpha;
    std::cout << "Default constructible: " << std::is_default_constructible<socket_t>::value << '\n';
    std::cout << "Copy constructible: " << std::is_copy_constructible<socket_t>::value << '\n';
    std::cout << "Move constructible: " << std::is_move_constructible<socket_t>::value << '\n';
    std::cout << "Copy assignable: " << std::is_copy_assignable<socket_t>::value << '\n';
    std::cout << "Move assignable: " << std::is_move_assignable<socket_t>::value << '\n';
}