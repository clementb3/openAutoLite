cmake_minimum_required(VERSION 3.10)
project(openAutoLite)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(openAutoLiteApp ${SOURCES})

target_include_directories(openAutoLiteApp PRIVATE include)

find_package(Threads REQUIRED)
target_link_libraries(openAutoLiteApp PRIVATE Threads::Threads)

install(TARGETS openAutoLiteApp DESTINATION bin)