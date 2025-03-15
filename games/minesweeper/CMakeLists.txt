cmake_minimum_required(VERSION "3.28.3")

project("Minesweeper")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(main 
    main.cpp
    guibuilder.h
    guibuilder.cpp
    minesweeper.cpp
    minesweeper.h
)

target_link_libraries(main raylib)