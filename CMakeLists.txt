cmake_minimum_required(VERSION 3.20)
set(APP_NAME Sampler)
project(APP_NAME LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# deps:
find_package(cxxopts REQUIRED)


# Application
set(APP_SOURCES
    ./src/main.cpp
    )
SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -ffast-math ${CMAKE_CXX_FLAGS_RELEASE}")

add_executable(${APP_NAME} ${APP_SOURCES})

target_link_libraries(${APP_NAME} PRIVATE cxxopts::cxxopts)
