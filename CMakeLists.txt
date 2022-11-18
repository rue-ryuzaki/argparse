cmake_minimum_required(VERSION 3.2)

include(cmake/prelude.cmake)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 7)
set(VERSION_PATCH 1)

if (${CMAKE_VERSION} VERSION_LESS "3.9")
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

set(PROJECT_HOMEPAGE_URL "https://github.com/rue-ryuzaki/argparse")

include(cmake/project-is-top-level.cmake)

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

include(GNUInstallDirs)

target_include_directories(
    argparse INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if (NOT PROJECT_IS_TOP_LEVEL)
    return()
endif()

install(FILES ${ARGPARSE_HEADER} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/argparse)

set(CPACK_PACKAGE_NAME "cpp-argparse-dev")
set(CPACK_PACKAGE_MAINTAINER "Mikhail Golubchikov")
set(CMAKE_PROJECT_HOMEPAGE_URL "${PROJECT_HOMEPAGE_URL}")
if (${CMAKE_VERSION} VERSION_LESS "3.12")
    set(CPACK_PACKAGE_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
endif()
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CPACK_GENERATOR DEB)
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
endif()

if (NOT CPACK_GENERATOR)
    if (WIN32)
        set(CPACK_GENERATOR ZIP)
    else()
        set(CPACK_GENERATOR TGZ)
    endif()
endif()

include(CPack)

enable_testing()
add_subdirectory(test)
