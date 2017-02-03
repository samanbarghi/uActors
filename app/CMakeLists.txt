# Define an executable
add_executable(app
    src/Main.cpp)
add_executable(ring2
    src/Ring2.cpp)


# Define the libraries this project depends upon
target_link_libraries(app   uActors)
target_link_libraries(ring2 uActors)