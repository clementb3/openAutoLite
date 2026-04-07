cmake_minimum_required(VERSION 3.10)
project(openAutoLite)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")

add_executable(openAutoLiteApp ${SOURCES} ${HEADERS})

target_include_directories(openAutoLiteApp PRIVATE 
    "${BASE_DIR}/include"
    "${BASE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
find_package(Threads REQUIRED)
target_link_libraries(openAutoLiteApp PRIVATE Threads::Threads)

install(TARGETS openAutoLiteApp DESTINATION bin)