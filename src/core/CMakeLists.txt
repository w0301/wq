
# Now we will specify sources from which will be library built.
aux_source_directory("common/" WQ_CORE_SOURCES)

# At the end we have to build library
add_library(${WQ_CORE_LIB_NAME} ${WQ_CORE_SOURCES})

# Setting version of shared libraries.
set_target_properties(${WQ_CORE_LIB_NAME} PROPERTIES SOVERSION ${WQ_FULL_VERSION})

# and set variable of all libraries.
set(WQ_BUILT_LIBS "${WQ_BUILT_LIBS} ${WQ_CORE_LIB_NAME}")

# Installing library.
if(UNIX)
	install(TARGETS ${WQ_CORE_LIB_NAME} LIBRARY DESTINATION lib)
else(NOT UNIX)
	# TODO: Windows destination dirs will be more complex.
endif(UNIX)

