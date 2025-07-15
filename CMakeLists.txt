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

set(CMAKE_CXX_STANDARD 17)
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

add_executable(${EXECUTABLE_NAME})

set_target_properties(${EXECUTABLE_NAME}

    PROPERTIES

    VERSION "${PROJECT_VERSION}")

target_include_directories(${EXECUTABLE_NAME}

    PUBLIC

    "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_sources(${EXECUTABLE_NAME}

    PUBLIC
    # Global
    "${CMAKE_CURRENT_SOURCE_DIR}/include/global.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/global.cpp"

    # Input
    "${CMAKE_CURRENT_SOURCE_DIR}/include/input.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/input.cpp"

    # Containers
    "${CMAKE_CURRENT_SOURCE_DIR}/include/containers.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/containers.cpp"

    # Database
    "${CMAKE_CURRENT_SOURCE_DIR}/include/database.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/database.cpp"

    # Finite State Machine
    "${CMAKE_CURRENT_SOURCE_DIR}/include/state_manager.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/state.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/main_menu_state.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/vessel_management_state.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/vessel_management_state.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/sailing_management_state.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/reservation_management_state.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/boarding_state.hpp"

    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

# [[ ----------------------------------------------------------------------- ]]
# [[ ----------------------------------------------------------------------- ]]
# [[ Executable Target ]]





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
