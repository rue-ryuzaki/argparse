macro(add_example EXAMPLE_NAME)
    set(unsupported "${CMAKE_CURRENT_SOURCE_DIR}/${EXAMPLE_NAME}/unsupported")
    if (EXISTS ${unsupported})
        file(READ ${unsupported} FILE_CONTENT)
        separate_arguments(FILE_CONTENT)
        foreach(LINE ${FILE_CONTENT})
            string(STRIP ${LINE} LINE)
            if ("${CMAKE_CXX_STANDARD}" STREQUAL "${LINE}")
                message(WARNING "Unsupported standard C++${CMAKE_CXX_STANDARD} for example: ${EXAMPLE_NAME}")
                return()
            endif()
        endforeach()
    endif()
    set(supported "${CMAKE_CURRENT_SOURCE_DIR}/${EXAMPLE_NAME}/supported")
    if (EXISTS ${supported})
        set(STANDARD_SUPPORTED FALSE)
        file(READ ${supported} FILE_CONTENT)
        separate_arguments(FILE_CONTENT)
        foreach(LINE ${FILE_CONTENT})
            string(STRIP ${LINE} LINE)
            if ("${CMAKE_CXX_STANDARD}" STREQUAL "${LINE}")
                set(STANDARD_SUPPORTED TRUE)
                break()
            endif()
        endforeach()
        if (NOT STANDARD_SUPPORTED)
            message(WARNING "Unsupported standard C++${CMAKE_CXX_STANDARD} for example: ${EXAMPLE_NAME}")
            return()
        endif()
    endif()

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
