include(${CMAKE_DIR}/Libraries.cmake)

add_library(
    SantasWorkshopLib
)

target_sources(
    SantasWorkshopLib
    PRIVATE
        Common.c
        Print.c
        Queue.c
        TaskGenerator.c
)

target_include_directories(
    SantasWorkshopLib 
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(
    TARGETS SantasWorkshopLib
    EXPORT SantasWorkshopLibraries-config
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(
    DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/SantasWorkshopLib
    FILES_MATCHING PATTERN "*.h"
)
