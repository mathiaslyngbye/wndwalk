cmake_minimum_required(VERSION 3.15)
project(wndwalk)

set(CMAKE_CXX_STANDARD 17)
add_definitions(-DUNICODE -D_UNICODE)

add_executable(wndwalk
    src/main.cpp
    src/key_manager.cpp
)

target_link_libraries(wndwalk
    user32
)

set_target_properties(wndwalk PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")

