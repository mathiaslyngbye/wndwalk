cmake_minimum_required(VERSION 3.15)
project(wndwalk)

add_definitions(-DUNICODE -D_UNICODE)

set(CMAKE_CXX_STANDARD 17)

add_executable(wndwalk
    src/main.cpp
)

target_link_libraries(wndwalk
    user32
)
