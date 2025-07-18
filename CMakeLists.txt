cmake_minimum_required(VERSION "3.22.1" FATAL_ERROR)


# [[ Project ]]
# [[ ----------------------------------------------------------------------- ]]
# [[ ----------------------------------------------------------------------- ]]

# NOTE (SAVIZ): Project name cannot contain spaces. Use underlines instead.

project("FerryFlow"

    VERSION 0.0.1

    DESCRIPTION "A project designed to streamline the management of ferry operations."

    LANGUAGES CXX C)

set(EXECUTABLE_NAME "${PROJECT_NAME}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# [[ ----------------------------------------------------------------------- ]]
# [[ ----------------------------------------------------------------------- ]]
# [[ Project ]]





# [[ Output Locations ]]
# [[ ----------------------------------------------------------------------- ]]
# [[ ----------------------------------------------------------------------- ]]

if(CMAKE_SIZEOF_VOID_P EQUAL 8)

    set(CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}/build/bin/${CMAKE_SYSTEM_NAME}/64-bit/")
endif()


if(CMAKE_SIZEOF_VOID_P EQUAL 4)

    set(CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}/build/bin/${CMAKE_SYSTEM_NAME}/32-bit/")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/executable") # NOTE (SAVIZ): Defines where executables will be located

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/executable") # NOTE (SAVIZ): Defines where shared libraries will be located

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/lib_static") # NOTE (SAVIZ): Defines where static libraries will be located

# [[ ----------------------------------------------------------------------- ]]
# [[ ----------------------------------------------------------------------- ]]
# [[ Output Locations ]]





# [[ Executable Target ]]
# [[ ----------------------------------------------------------------------- ]]
# [[ ----------------------------------------------------------------------- ]]

set(HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/global.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/input.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/containers.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/database.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/state_manager.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/state.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/main_menu_state.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/vessel_management_state.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/sailing_management_state.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/reservation_management_state.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/boarding_state.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/utilities.hpp"
)

set(SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/global.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/input.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/containers.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/database.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/state_manager.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/vessel_management_state.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/boarding_state.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/reservation_management_state.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/sailing_management_state.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/utilities.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/state.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main_menu_state.cpp"
)

add_executable(${EXECUTABLE_NAME})

set_target_properties(${EXECUTABLE_NAME}

    PROPERTIES

    VERSION "${PROJECT_VERSION}")

target_include_directories(${EXECUTABLE_NAME}

    PUBLIC

    "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_sources(${EXECUTABLE_NAME}
    PUBLIC
        ${HEADERS}
    PRIVATE
        ${SOURCES}
        "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
)

# [[ ----------------------------------------------------------------------- ]]
# [[ ----------------------------------------------------------------------- ]]
# [[ Executable Target ]]





# [[ External libraries ]]
# [[ ----------------------------------------------------------------------- ]]
# [[ ----------------------------------------------------------------------- ]]

include(FetchContent)

# Catch2 (Unit testing)
FetchContent_Declare(Catch2

    GIT_REPOSITORY "https://github.com/catchorg/Catch2.git"
    GIT_TAG "v3.4.0"
    GIT_SHALLOW TRUE)

FetchContent_MakeAvailable(Catch2)

include(CTest)
include(Catch)
set(CMAKE_CATCH_DISCOVER_TESTS_DISCOVERY_MODE PRE_TEST)

target_link_libraries(${EXECUTABLE_NAME} PRIVATE Catch2::Catch2WithMain)

# [[ ----------------------------------------------------------------------- ]]
# [[ ----------------------------------------------------------------------- ]]
# [[ External libraries ]]





# [[ Compile definitions ]]
# [[ ----------------------------------------------------------------------- ]]
# [[ ----------------------------------------------------------------------- ]]

# NOTE (SAVIZ): I realize that using the 'target' variation of this command is generally preferred, but since I need the effects to be global, I opted for this version instead.

add_compile_definitions(

    $<$<PLATFORM_ID:Linux>:TARGET_IS_LINUX>
    $<$<PLATFORM_ID:Darwin>:TARGET_IS_APPLE>
    $<$<PLATFORM_ID:Windows>:TARGET_IS_WINDOWS>)


add_compile_definitions(

    $<$<CONFIG:Debug>:DEBUG_MODE>
    $<$<CONFIG:Release>:RELEASE_MODE>
    $<$<CONFIG:RelWithDebInfo>:RELEASE_WITH_DEBUGINFO_MODE>
    $<$<CONFIG:MinSizeRel>:RELEASE_MINSIZE_MODE>)

# [[ ----------------------------------------------------------------------- ]]
# [[ ----------------------------------------------------------------------- ]]
# [[ Compile definitions ]]





# [[ Libraries ]]
# [[ ----------------------------------------------------------------------- ]]
# [[ ----------------------------------------------------------------------- ]]

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/libs")

target_link_libraries(${EXECUTABLE_NAME}

    PRIVATE
    "Lib_SQLite3")

# [[ ----------------------------------------------------------------------- ]]
# [[ ----------------------------------------------------------------------- ]]
# [[ Libraries ]]





# [[ Adding unit tests ]]
# [[ ----------------------------------------------------------------------- ]]
# [[ ----------------------------------------------------------------------- ]]

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/unit_tests")

# [[ ----------------------------------------------------------------------- ]]
# [[ ----------------------------------------------------------------------- ]]
# [[ Adding unit tests]]
