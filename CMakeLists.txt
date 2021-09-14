cmake_minimum_required(VERSION 3.2)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 4)
set(VERSION_PATCH 4)

project("ArgumentParser"
    DESCRIPTION "C++11 argument parser"
    VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
)

if(NOT MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wold-style-cast -Wextra -Wundef -Wsign-compare -Wconversion -Wpointer-arith -Weffc++ -Wnon-virtual-dtor")
endif()

set(CMAKE_CXX_STANDARD 11)

add_library(argparse INTERFACE)

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
include(CPack)

enable_testing()
add_subdirectory(test)
