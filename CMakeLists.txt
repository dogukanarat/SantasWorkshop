cmake_minimum_required(VERSION 3.6)

project(
    SantasWorkshop
    VERSION 0.1
    LANGUAGES C CXX
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -lpthread")


if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(SCRIPT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Scripts")
set(CMAKE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/CMake")

include(GNUInstallDirs)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

add_subdirectory(Externals)
add_subdirectory(Libraries)
add_subdirectory(Applications)

enable_testing()

add_subdirectory(Tests)
