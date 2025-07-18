# [[ Project ]]
# [[ ----------------------------------------------------------------------- ]]
# [[ ----------------------------------------------------------------------- ]]

# NOTE (SAVIZ): Each unit test can act as a project. Project name cannot contain any spaces. Use underlines instead.
project("Test_Containers"

    VERSION 0.0.1

    DESCRIPTION "A simple unit test to make sure that the
                 containers module is working correctly."

    LANGUAGES CXX)

set(EXECUTABLE_NAME "${PROJECT_NAME}")

# [[ ----------------------------------------------------------------------- ]]
# [[ ----------------------------------------------------------------------- ]]
# [[ Project ]]





# [[ Files ]]
# [[ ----------------------------------------------------------------------- ]]
# [[ ----------------------------------------------------------------------- ]]

set(HEADERS
    "${CMAKE_SOURCE_DIR}/include/containers.hpp"
    "${CMAKE_SOURCE_DIR}/include/utilities.hpp"
)

set(SOURCES
    "${CMAKE_SOURCE_DIR}/src/containers.cpp"
    "${CMAKE_SOURCE_DIR}/src/utilities.cpp"
)

set(TEST_FILES

    "${CMAKE_CURRENT_SOURCE_DIR}/src/test_containers.cpp")

# [[ ----------------------------------------------------------------------- ]]
# [[ ----------------------------------------------------------------------- ]]
# [[ Files ]]





# [[ Test ]]
# [[ ----------------------------------------------------------------------- ]]
# [[ ----------------------------------------------------------------------- ]]

add_executable(${EXECUTABLE_NAME})

# NOTE (SAVIZ): This action makes it easier to distinguish between main executable and unit tests.
set_target_properties(${EXECUTABLE_NAME}
    
    PROPERTIES
    
    VERSION "${PROJECT_VERSION}")

# NOTE (SAVIZ): Tests usually contain well... tests! So I don't see the point of exposing header files to the rest of the project. As a result, header files are private.
target_include_directories(${EXECUTABLE_NAME}

    PRIVATE

    "${CMAKE_SOURCE_DIR}/include")

target_sources(${EXECUTABLE_NAME}
    PRIVATE
        ${HEADERS}
        ${SOURCES}
        ${TEST_FILES})

target_link_libraries(${EXECUTABLE_NAME}

    PRIVATE
    Catch2::Catch2WithMain
    "Lib_SQLite3")

# [[ ----------------------------------------------------------------------- ]]
# [[ ----------------------------------------------------------------------- ]]
# [[ Test ]]





# [[ Discover Test ]]
# [[ ----------------------------------------------------------------------- ]]
# [[ ----------------------------------------------------------------------- ]]

catch_discover_tests(${EXECUTABLE_NAME})

# [[ ----------------------------------------------------------------------- ]]
# [[ ----------------------------------------------------------------------- ]]
# [[ Discover Test ]]
