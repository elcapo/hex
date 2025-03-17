cmake_minimum_required(VERSION 3.10)

project(Hex LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

enable_testing()

add_subdirectory(src)
add_subdirectory(tests)
