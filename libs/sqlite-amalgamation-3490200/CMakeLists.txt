# [[ Project ]]
# [[ ----------------------------------------------------------------------- ]]
# [[ ----------------------------------------------------------------------- ]]

# NOTE (SAVIZ): Each internal library can act as a project. Project name cannot contain any spaces. Use underlines instead.
project("Lib_SQLite3"

    VERSION 0.0.1

    DESCRIPTION "An amalgamation version of sqlite3."

    LANGUAGES CXX C)

set(LIBRARY_NAME "${PROJECT_NAME}")

# [[ ----------------------------------------------------------------------- ]]
# [[ ----------------------------------------------------------------------- ]]
# [[ Project ]]





# [[ Files ]]
# [[ ----------------------------------------------------------------------- ]]
# [[ ----------------------------------------------------------------------- ]]

set(HEADER_FILES

    "${CMAKE_CURRENT_SOURCE_DIR}/include/sqlite3.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/sqlite3ext.h")
	

set(SOURCE_FILES

    "${CMAKE_CURRENT_SOURCE_DIR}/src/shell.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/sqlite3.c")

# [[ ----------------------------------------------------------------------- ]]
# [[ ----------------------------------------------------------------------- ]]
# [[ Files ]]





# [[ Library ]]
# [[ ----------------------------------------------------------------------- ]]
# [[ ----------------------------------------------------------------------- ]]

# NOTE (SAVIZ): Static libraries are easier to manage because they are embedded directly into the final executable, eliminating issues related to missing shared libraries at runtime.
add_library(${LIBRARY_NAME} STATIC)

set_target_properties(${LIBRARY_NAME}

    PROPERTIES

    VERSION "${PROJECT_VERSION}")

target_sources(${LIBRARY_NAME}

    PUBLIC
    ${HEADER_FILES}

    PRIVATE
    ${SOURCE_FILES})

target_include_directories(${LIBRARY_NAME}
	
    PUBLIC

    "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(${LIBRARY_NAME}

    PRIVATE)

# [[ ----------------------------------------------------------------------- ]]
# [[ ----------------------------------------------------------------------- ]]
# [[ Library ]]
