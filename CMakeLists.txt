cmake_minimum_required(VERSION 3.2)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 3)
set(VERSION_PATCH 2)

project("ArgumentParser"
    DESCRIPTION "C++11 argument parser"
    VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /EHsc")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wold-style-cast -Wextra -Wundef -Wsign-compare -Wconversion -Wpointer-arith -Weffc++ -Wnon-virtual-dtor -Wsign-conversion")
endif()

if (NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 11)
endif()

add_library(argparse INTERFACE)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set_target_properties(argparse PROPERTIES
        PUBLIC_HEADER include/argparse/argparse.hpp
    )

    include(GNUInstallDirs)
    install(TARGETS argparse PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/argparse)

    set(CPACK_GENERATOR "DEB")
    set(CPACK_PACKAGE_NAME "cpp-argparse-dev")
    set(CPACK_PACKAGE_MAINTAINER "Mikhail Golubchikov")
    set(CMAKE_PROJECT_HOMEPAGE_URL "https://github.com/rue-ryuzaki/argparse")
    set(CPACK_DEBIAN_PACKAGE_NAME "${CPACK_PACKAGE_NAME}")
    set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "Python-like header-only C++11 argument parser")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_MAINTAINER}")
    set(CPACK_DEBIAN_PACKAGE_SUGGESTS "cmake, build-essential")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
    set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

    set(CPACK_RPM_PACKAGE_NAME "${CPACK_PACKAGE_NAME}")
    set(CPACK_RPM_PACKAGE_DESCRIPTION "${CPACK_DEBIAN_PACKAGE_DESCRIPTION}")
    set(CPACK_RPM_PACKAGE_MAINTAINER "${CPACK_PACKAGE_MAINTAINER}")
    set(CPACK_RPM_PACKAGE_SUGGESTS "${CPACK_DEBIAN_PACKAGE_SUGGESTS}")
    set(CPACK_RPM_PACKAGE_LICENSE "MIT")

    set(CPACK_DEB_COMPONENT_INSTALL ON)
    set(CPACK_RPM_COMPONENT_INSTALL ON)
    include(CPack)
endif()

enable_testing()
add_subdirectory(test)
