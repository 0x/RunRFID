cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(RunRFID)

find_package(PkgConfig REQUIRED) 

file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra")
pkg_check_modules(PA_PKG REQUIRED IMPORTED_TARGET libserialport)

add_executable(RunRFID src/main.cpp src/RunRFID.cpp)

target_link_libraries(RunRFID PkgConfig::PA_PKG)

