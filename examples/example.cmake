macro(add_example EXAMPLE_NAME)
    message(STATUS "Add example: ${EXAMPLE_NAME}")

    file(GLOB_RECURSE EXAMPLE_SOURCES "${EXAMPLE_NAME}/src/*.c*" "${EXAMPLE_NAME}/include/*.h*")

    add_executable(${EXAMPLE_NAME} $<TARGET_OBJECTS:argparse_impl> ${EXAMPLE_SOURCES})
    target_include_directories(${EXAMPLE_NAME} PRIVATE ${EXAMPLE_NAME}/include)

    if (ARGPARSE_BUILD_TESTS AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${EXAMPLE_NAME}/tests)
        message(STATUS "Build ${EXAMPLE_NAME} unit tests")

        enable_testing()
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/${EXAMPLE_NAME}/tests ${EXAMPLE_NAME}-tests)
    endif()
endmacro()
