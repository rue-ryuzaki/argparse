include(cmake/prelude.cmake)

if (${CMAKE_VERSION} VERSION_LESS "3.12")
    cmake_minimum_required(VERSION 3.0)
else()
    cmake_minimum_required(VERSION 3.0...3.12)
endif()

file(READ include/argparse/argparse.hpp CURRENT_VERSION_H)
string(REGEX MATCH "\#define ARGPARSE_VERSION_MAJOR +([0-9]+)" MAJOR_VERSION ${CURRENT_VERSION_H})
set(LIBRARY_VERSION_MAJOR ${CMAKE_MATCH_1})
string(REGEX MATCH "\#define ARGPARSE_VERSION_MINOR +([0-9]+)" MINOR_VERSION ${CURRENT_VERSION_H})
set(LIBRARY_VERSION_MINOR ${CMAKE_MATCH_1})
string(REGEX MATCH "\#define ARGPARSE_VERSION_PATCH +([0-9]+)" PATCH_VERSION ${CURRENT_VERSION_H})
set(LIBRARY_VERSION_PATCH ${CMAKE_MATCH_1})

project("ArgumentParser"
    VERSION ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}
    LANGUAGES CXX
)

set(PROJECT_HOMEPAGE_URL "https://github.com/rue-ryuzaki/argparse")

include(cmake/project-is-top-level.cmake)
include(cmake/warnings.cmake)

if (NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 11)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(LIBRARY_NAME argparse)

message(STATUS "${LIBRARY_NAME} version: ${PROJECT_VERSION}")
message(STATUS "${LIBRARY_NAME} build standard: C++${CMAKE_CXX_STANDARD}")

file(GLOB LIBRARY_HEADER "include/argparse/argparse*.hpp")

if (${CMAKE_VERSION} VERSION_LESS "3.19")
    add_library(${LIBRARY_NAME} INTERFACE)
else()
    add_library(${LIBRARY_NAME} INTERFACE ${LIBRARY_HEADER})
endif()
add_library(argparse::${LIBRARY_NAME} ALIAS ${LIBRARY_NAME})

include(GNUInstallDirs)

target_include_directories(
    ${LIBRARY_NAME} INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

set(STATIC_LIBRARY_NAME ${LIBRARY_NAME}_static)

option(ARGPARSE_STATIC "Build ${LIBRARY_NAME} as a static lib ${STATIC_LIBRARY_NAME}" OFF)
option(ARGPARSE_STATIC_PIC "Build ${STATIC_LIBRARY_NAME} with -fPIC" ON)

if (ARGPARSE_STATIC)
    message(STATUS "Build ${LIBRARY_NAME} as a static lib ${STATIC_LIBRARY_NAME}")

    add_library(${STATIC_LIBRARY_NAME} STATIC src/argparse_impl.cpp)
    add_library(argparse::${STATIC_LIBRARY_NAME} ALIAS ${STATIC_LIBRARY_NAME})

    foreach(DEFINITION ${ARGPARSE_STATIC_DEFINITIONS})
        if (NOT ${DEFINITION} STREQUAL "ARGPARSE_DECLARATION")
            target_compile_definitions(${STATIC_LIBRARY_NAME} PRIVATE ${DEFINITION})
        endif()
    endforeach()

    if (ARGPARSE_STATIC_PIC)
        message(STATUS "Build ${STATIC_LIBRARY_NAME} with -fPIC")
        set_property(
            TARGET ${STATIC_LIBRARY_NAME}
            PROPERTY POSITION_INDEPENDENT_CODE ON
        )
    endif()

    target_include_directories(
        ${STATIC_LIBRARY_NAME} PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )
endif()

if (NOT PROJECT_IS_TOP_LEVEL)
    return()
endif()

file(GLOB LIBRARY_MARKDOWN "*.md")
add_custom_target(${LIBRARY_NAME}.md SOURCES ${LIBRARY_MARKDOWN})

if (DEFERED_CMAKE_INSTALL_PREFIX)
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/pkg-config.pc.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cpp-argparse.pc" @ONLY
    )
else()
    set(_MARK "@")
    set(DEFERED_CMAKE_INSTALL_PREFIX "${_MARK}CMAKE_INSTALL_PREFIX${_MARK}")

    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/pkg-config.pc.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cpp-argparse.pc.in" @ONLY
    )
    install(
        CODE "configure_file(\"${CMAKE_CURRENT_BINARY_DIR}/cpp-argparse.pc.in\" \"${CMAKE_CURRENT_BINARY_DIR}/cpp-argparse.pc\" @ONLY)"
    )
endif()

set(LIBRARY_CONFIG "${CMAKE_CURRENT_BINARY_DIR}/cpp-argparse.pc")

install(FILES ${LIBRARY_HEADER} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/argparse")
install(FILES ${LIBRARY_CONFIG} DESTINATION "${CMAKE_INSTALL_DATADIR}/pkgconfig")

set(CPACK_PACKAGE_NAME "cpp-argparse-dev")
set(CPACK_PACKAGE_MAINTAINER "Mikhail Golubchikov")
set(CMAKE_PROJECT_HOMEPAGE_URL "${PROJECT_HOMEPAGE_URL}")
if (${CMAKE_VERSION} VERSION_LESS "3.12")
    set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
endif()
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CPACK_RESOURCE_FILE_README}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "C++ argument parser")

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CPACK_GENERATOR "DEB;RPM")
    ## DEB
    set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
    set(CPACK_DEBIAN_PACKAGE_NAME "${CPACK_PACKAGE_NAME}")
    set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "Python-like header-only C++ argument parser")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_MAINTAINER}")
    # set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/CHANGELOG.md")
    if (NOT ${CMAKE_VERSION} VERSION_LESS "3.6")
        set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
    endif()
    ## RPM
    set(CPACK_RPM_PACKAGE_NAME "${CPACK_PACKAGE_NAME}")
    set(CPACK_RPM_PACKAGE_DESCRIPTION "${CPACK_DEBIAN_PACKAGE_DESCRIPTION}")
    set(CPACK_RPM_PACKAGE_MAINTAINER "${CPACK_PACKAGE_MAINTAINER}")
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

option(ARGPARSE_BUILD_EXAMPLES "Build ${LIBRARY_NAME} examples" OFF)
option(ARGPARSE_BUILD_TESTS "Build ${LIBRARY_NAME} unit tests" ON)

if (ARGPARSE_BUILD_EXAMPLES OR ARGPARSE_BUILD_TESTS)
    # argparse implementation (internal)
    set(LIBRARY_IMPL_NAME ${LIBRARY_NAME}_impl)
    add_library(
        ${LIBRARY_IMPL_NAME} OBJECT
        "${CMAKE_CURRENT_SOURCE_DIR}/src/argparse_impl.cpp"
    )
    target_include_directories(
        ${LIBRARY_IMPL_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
    )
    set_target_properties(${LIBRARY_IMPL_NAME} PROPERTIES
        COMPILE_DEFINITIONS "$<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS> ARGPARSE_DISABLE_UTILS"
        COMPILE_OPTIONS "$<$<CXX_COMPILER_ID:MSVC>:/EHsc;$<$<CONFIG:Release>:/Od>>"
    )

    set(ARGPARSE_THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party")

    if (ARGPARSE_BUILD_EXAMPLES)
        message(STATUS "Build ${LIBRARY_NAME} examples")

        add_subdirectory(examples)
    endif()

    if (ARGPARSE_BUILD_TESTS)
        message(STATUS "Build ${LIBRARY_NAME} unit tests")

        enable_testing()
        add_subdirectory(tests)
    endif()
endif()
