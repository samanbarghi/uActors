# At LEAST 2.8 but newer is better
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(uActors VERSION 0.1 LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 14)

# External 3rd party libs that we include
add_subdirectory(libs/libcuckoo EXCLUDE_FROM_ALL)

# Set include dirs and libraries for the executable.
include_directories(libs/libcuckoo)

# Targets that we develop
enable_testing()
add_subdirectory(uActors)
add_subdirectory(app)