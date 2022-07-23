cmake_minimum_required(VERSION 3.2)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 6)
set(VERSION_PATCH 6)

if (${CMAKE_VERSION} VERSION_LESS "3.8")
    project("ArgumentParser"
        VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
        LANGUAGES CXX
    )
else()
    project("ArgumentParser"
        DESCRIPTION "C++ argument parser"
        VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
        LANGUAGES CXX
    )
endif()

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /EHsc")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wold-style-cast -Wextra -Wundef -Wsign-compare -Wconversion -Wpointer-arith -Weffc++ -Wnon-virtual-dtor -Wsign-conversion")
endif()

if (NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 11)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ARGPARSE_HEADER include/argparse/argparse.hpp)

if (${CMAKE_VERSION} VERSION_LESS "3.19")
    add_library(argparse INTERFACE)
else()
    add_library(argparse INTERFACE ${ARGPARSE_HEADER})
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    include(GNUInstallDirs)

    target_include_directories(
        argparse INTERFACE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )

    install(FILES ${ARGPARSE_HEADER} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/argparse)

    set(CPACK_GENERATOR "DEB")
    set(CPACK_PACKAGE_NAME "cpp-argparse-dev")
    set(CPACK_PACKAGE_MAINTAINER "Mikhail Golubchikov")
    set(CMAKE_PROJECT_HOMEPAGE_URL "https://github.com/rue-ryuzaki/argparse")
    if (${CMAKE_VERSION} VERSION_LESS "3.12")
        set(CPACK_PACKAGE_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
    endif()
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
    set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
    ## DEB
    set(CPACK_DEBIAN_PACKAGE_NAME "${CPACK_PACKAGE_NAME}")
    set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "Python-like header-only C++ argument parser")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_MAINTAINER}")
    set(CPACK_DEBIAN_PACKAGE_SUGGESTS "cmake, build-essential")
    if (NOT ${CMAKE_VERSION} VERSION_LESS "3.6")
        set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
    endif()
    ## RPM
    set(CPACK_RPM_PACKAGE_NAME "${CPACK_PACKAGE_NAME}")
    set(CPACK_RPM_PACKAGE_DESCRIPTION "${CPACK_DEBIAN_PACKAGE_DESCRIPTION}")
    set(CPACK_RPM_PACKAGE_MAINTAINER "${CPACK_PACKAGE_MAINTAINER}")
    set(CPACK_RPM_PACKAGE_SUGGESTS "${CPACK_DEBIAN_PACKAGE_SUGGESTS}")
    set(CPACK_RPM_PACKAGE_LICENSE "MIT")
    if (NOT ${CMAKE_VERSION} VERSION_LESS "3.6")
        set(CPACK_RPM_FILE_NAME RPM-DEFAULT)
    endif()

    set(CPACK_DEB_COMPONENT_INSTALL ON)
    set(CPACK_RPM_COMPONENT_INSTALL ON)
    include(CPack)
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    include(GNUInstallDirs)

    target_include_directories(
        argparse INTERFACE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )

    install(FILES ${ARGPARSE_HEADER} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/argparse)
endif()

enable_testing()
add_subdirectory(test)
