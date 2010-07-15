
# Setting output path for executable.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")

# Setting sources for sample.
set(TABLES_GEN_SOURCES "main.cpp")

# Building executable of sample and linking needed libraries.
add_executable(tables_gen ${TABLES_GEN_SOURCES})
target_link_libraries(tables_gen ${WQ_CORE_LIB_NAME})
