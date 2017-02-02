# Define library. Only source files here!
project(libuActors VERSION 0.1 LANGUAGES CXX)

add_library(uActors SHARED
    src/Message.cpp
    src/ProcessRegistry.cpp
    src/Spawn.cpp)

# Define headers for this library. PUBLIC headers are used for
# compiling the library, and will be added to consumers' build
# paths.
target_include_directories(uActors PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE src)

# If we have compiler requirements for this library, list them
# here
target_compile_features(uActors
    PRIVATE cxx_range_for)
#    PUBLIC cxx_auto_type
#    PRIVATE cxx_variadic_templates)

# Depend on a library that we defined in the top-level file
target_link_libraries(uActors
    cityhash uThreads)

# 'make install' to the correct location
install(TARGETS uActors EXPORT uActorsConfig
    ARCHIVE  DESTINATION lib
    LIBRARY  DESTINATION lib
    RUNTIME  DESTINATION bin)  # This is for Windows
install(DIRECTORY include/ DESTINATION include)

# This makes the project importable from the install directory
# Put config file in per-project dir (name MUST match), can also
# just go into <prefix>/cmake.
install(EXPORT uActorsConfig DESTINATION share/MyLibrary/cmake)

# This makes the project importable from the build directory
export(TARGETS uActors FILE uActorsConfig.cmake)

# Every library has unit tests, of course
#add_executable(testlib
#    test/testlib.cpp)

#target_link_libraries(testlib
#    lib)

#add_test(testlib testlib)