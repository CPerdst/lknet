cmake_minimum_required(VERSION 3.24)

set(ABSEIL_INCLUDE_DIR "/opt/homebrew/opt/abseil/include")
set(ABSEIL_LIB_DIR "/opt/homebrew/opt/abseil/lib")
set(PROTOBUF_INCLUDE_DIR "/opt/homebrew/opt/protobuf/include")
set(PROTOBUF_LIB_DIR "/opt/homebrew/opt/protobuf/lib")

project(server)

set(CMAKE_CXX_STANDARD 17)

find_package(Protobuf REQUIRED)

file(GLOB ABSEIL_LIBS "/opt/homebrew/opt/abseil/lib/lib*.dylib")
file(GLOB LKNET_SOURCE "./lknet/source/*.cpp")
file(GLOB LKLOG_SOURCE "./logger/*.cpp")

protobuf_generate_cpp(REQUEST_PROTO_SRC REQUEST_PROTO_HDR "./common/proto/request.proto")

add_executable(server main.cpp ${LKNET_SOURCE} ${LKLOG_SOURCE} ${REQUEST_PROTO_SRC} ${REQUEST_PROTO_HDR})

target_include_directories(server PRIVATE /Users/zwj1/Downloads/boost_1_73_0 ./lknet/include ./logger ${ABSEIL_INCLUDE_DIR} ${PROTOBUF_INCLUDE_DIR} )
target_link_directories(server PRIVATE /Users/zwj1/Downloads/boost_1_73_0/stage/lib ${ABSEIL_LIB_DIR} ${PROTOBUF_LIB_DIR})
target_link_libraries(server boost_system boost_filesystem boost_thread boost_date_time boost_serialization boost_log boost_log_setup ${ABSEIL_LIBS} protobuf protobuf-lite)






project(client)

set(CMAKE_CXX_STANDARD 17)

find_package(Protobuf REQUIRED)

file(GLOB ABSEIL_LIBS "/opt/homebrew/opt/abseil/lib/lib*.dylib")
file(GLOB LKNET_SOURCE "./lknet/source/*.cpp")
file(GLOB LKLOG_SOURCE "./logger/*.cpp")

protobuf_generate_cpp(REQUEST_PROTO_SRC REQUEST_PROTO_HDR "./common/proto/request.proto")

add_executable(client main.cpp ${LKNET_SOURCE} ${LKLOG_SOURCE} ${REQUEST_PROTO_SRC} ${REQUEST_PROTO_HDR})

message(STATUS "REQUEST_PROTO_HDR: ${REQUEST_PROTO_HDR}")

target_include_directories(client PRIVATE /Users/zwj1/Downloads/boost_1_73_0 ./lknet/include ./logger ${ABSEIL_INCLUDE_DIR} ${PROTOBUF_INCLUDE_DIR})
target_link_directories(client PRIVATE /Users/zwj1/Downloads/boost_1_73_0/stage/lib ${ABSEIL_LIB_DIR} ${PROTOBUF_LIB_DIR})
target_link_libraries(client boost_system boost_filesystem boost_thread boost_date_time boost_serialization boost_log boost_log_setup ${ABSEIL_LIBS} protobuf protobuf-lite)
