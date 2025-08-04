cmake_minimum_required(VERSION 3.10)
project(HelloWordSFML)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

file(GLOB SOURCES "src/*.cpp")

add_executable(HelloWordSFML ${SOURCES})


target_include_directories(HelloWordSFML PRIVATE 
    "${PROJECT_SOURCE_DIR}/include"
)

target_link_libraries(HelloWordSFML sfml-graphics sfml-window sfml-system)

