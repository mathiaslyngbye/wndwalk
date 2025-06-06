cmake_minimum_required(VERSION 3.15)
project(wndwalk)

add_definitions(-DUNICODE -D_UNICODE)

add_executable(wndwalk
    src/main.cpp
)

target_link_libraries(wndwalk
    user32
    kernel32
)

# No console window
set_target_properties(wndwalk PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
