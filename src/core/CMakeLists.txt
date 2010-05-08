
# First of all we will set name of output library here.
set(WQ_CORE_LIB_NAME "wqCore${WQ_OUTPUT_LIBS_SUFFIX}")

# Now we will specify sources from which will be library built.
aux_source_directory("common/" WQ_CORE_SOURCES)

# At the end we have to build library
add_library(${WQ_CORE_LIB_NAME} ${WQ_CORE_SOURCES})

# and set variable of all libraries.
set(WQ_BUILT_LIBS "${WQ_BUILT_LIBS} ${WQ_CORE_LIB_NAME}")

