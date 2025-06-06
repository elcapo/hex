if (BUILD_TESTING)

cmake_policy(SET CMP0135 NEW)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(
    unit_test
    unit_test.cpp
    ../src/common.cpp
    ../src/strategy.cpp
    ../src/board.cpp
    ../src/ai.cpp
    ../src/dijkstra.cpp
    ../src/graph.cpp
)

target_link_libraries(
    unit_test
    GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(unit_test)

endif()