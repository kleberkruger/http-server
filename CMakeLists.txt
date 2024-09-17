cmake_minimum_required(VERSION 3.29)
project(http-server)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
        argparse
        GIT_REPOSITORY https://github.com/p-ranav/argparse
        GIT_TAG 68fd027
)
FetchContent_MakeAvailable(argparse)

FetchContent_Declare(
        yaml-cpp
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
        GIT_TAG 0.8.0 # Can be a tag (yaml-cpp-x.x.x), a commit hash, or a branch name (master)
)
FetchContent_MakeAvailable(yaml-cpp)

add_executable(http-server main.cpp
        http_server_config.cpp
        http_server_config.h
        http_server.cpp
        http_server.h
        http_server_mode.h)
target_link_libraries(http-server PRIVATE argparse::argparse yaml-cpp::yaml-cpp)
