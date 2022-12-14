add_subdirectory(SantasWorkshop)

install(
    EXPORT SantasWorkshop-config
    NAMESPACE SantasWorkshop::
    DESTINATION ${CMAKE_INSTALL_PREFIX}/cmake
)
