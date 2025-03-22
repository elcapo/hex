find_package(Curses REQUIRED)

include_directories(${CURSES_INCLUDE_DIR})

add_executable(hex main.cpp graph.cpp board.cpp)

target_link_libraries(hex ${CURSES_LIBRARIES})