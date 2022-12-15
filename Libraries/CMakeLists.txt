add_subdirectory(SantasWorkshopLib)

include_directories(${CMAKE_CURRENT_LIST_DIR})

install(
    EXPORT SantasWorkshopLibraries-config
    NAMESPACE SantasWorkshopLibraries::
    DESTINATION ${CMAKE_INSTALL_PREFIX}/cmake
)
