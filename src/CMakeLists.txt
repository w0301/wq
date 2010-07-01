
# We will have one library for each subdirectory in this folder - one
# subdirectory for each nested namespace of ::wq namespace.
# Hint: variable WQ_xxx_LIB_NAME contains target name of xxx library.

# We have to set definition which indicate that libs are beeing built.
add_definitions(-DWQ_BUILDING)

# First directory is for ::wq namespace and for first wqCore library.
add_subdirectory(core/)

