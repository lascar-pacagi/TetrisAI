cmake_minimum_required(VERSION 3.16)

project(TetrisAI
        VERSION 1.0
        LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -ggdb -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")

include_directories("include")

add_executable(play src/control.cpp src/piece.cpp src/shape.cpp src/tetris.cpp src/view.cpp src/play.cpp)
add_executable(learn src/control.cpp src/piece.cpp src/shape.cpp src/tetris.cpp src/view.cpp src/cem.cpp src/learn.cpp)

target_link_libraries(play sfml-graphics sfml-window sfml-system)
target_link_libraries(learn sfml-graphics sfml-window sfml-system tbb)
