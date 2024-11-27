cmake_minimum_required(VERSION 3.29)
project(gpt_net)

set(CMAKE_CXX_STANDARD 17)

add_executable(gpt_net main.cpp IOBase.cpp IOBase.h Message.h Client.cpp Client.h Server.cpp Server.h)

target_include_directories(gpt_net PRIVATE /Users/zwj1/Downloads/boost_1_73_0 ./lknet/include ./logger )
target_link_directories(gpt_net PRIVATE /Users/zwj1/Downloads/boost_1_73_0/stage/lib)
target_link_libraries(gpt_net boost_system boost_filesystem boost_thread boost_date_time boost_serialization boost_log boost_log_setup)



