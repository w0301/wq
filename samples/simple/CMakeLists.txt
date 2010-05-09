
# Setting output path for executable.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")

# Setting sources for sample.
set(SIMPLE_SOURCES "main.cpp")

# Building executable of sample and linking needed libraries.
add_executable(simple ${SIMPLE_SOURCES})
target_link_libraries(simple ${WQ_CORE_LIB_NAME})
