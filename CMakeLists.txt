cmake_minimum_required(VERSION 3.10)
project(BeatEmUp)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
link_directories(${SDL2_LIBRARY_DIRS})

add_executable(main main.cpp Game.cpp Player.cpp Enemy.cpp Stage.cpp Input.cpp)

target_link_libraries(main ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
