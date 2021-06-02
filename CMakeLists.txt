cmake_minimum_required(VERSION 2.8) 
project(snake)
set(CMAKE_C_FLAGS "-std=gnu99")
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(snake main.c charbuffer.c)
target_link_libraries(snake ${CURSES_LIBRARIES})