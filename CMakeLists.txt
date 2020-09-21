cmake_minimum_required(VERSION 2.8) 
project(main)
set(CMAKE_C_FLAGS "-std=gnu99")
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(main main.c charbuffer.c)
target_link_libraries(main ${CURSES_LIBRARIES})