cmake_minimum_required(VERSION 3.16)
project(Socket)

set(CMAKE_CXX_STANDARD 14)

add_executable(Socket main.cpp src/socket_address.cpp)
target_link_libraries(Socket ws2_32)