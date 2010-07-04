
# Setting output path for executable.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")

# Setting sources for sample.
set(UNI_TABLES_SOURCES "main.cpp")

# Building executable of sample and linking needed libraries.
add_executable(uni_tables ${UNI_TABLES_SOURCES})
target_link_libraries(uni_tables ${WQ_CORE_LIB_NAME})

