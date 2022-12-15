include(${CMAKE_DIR}/Applications.cmake)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(
    SantasWorkshop
)

target_link_libraries(
    SantasWorkshop
    PRIVATE
        SantasWorkshopLib
)

target_sources(
    SantasWorkshop 
    PRIVATE
        Main.c
        ${CUSTOM_HEADER}
)

install(
    TARGETS SantasWorkshop
    EXPORT SantasWorkshop-config
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})


