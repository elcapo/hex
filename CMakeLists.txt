cmake_minimum_required(VERSION 3.10)

project(Hex LANGUAGES CXX)

#set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)

add_subdirectory(src)
add_subdirectory(tests)
