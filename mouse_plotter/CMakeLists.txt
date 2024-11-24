cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -s")

# Set the project name
project(mouse_plotter)

# Find ncurses library
find_package(Curses REQUIRED)
include_directories(include ${CURSES_INCLUDE_DIR})

# Add the executable
add_executable(${CMAKE_PROJECT_NAME} 
	src/main.c
	src/mouse_plotter_functions.c)

# Link ncurses to your executable
target_link_libraries(${CMAKE_PROJECT_NAME} ${CURSES_LIBRARIES})
