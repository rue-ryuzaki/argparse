include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")

# catch main library
set(CATCH_OBJECT_NAME ${LIBRARY_NAME}_catch)
add_library(
    ${CATCH_OBJECT_NAME} OBJECT "${CMAKE_CURRENT_SOURCE_DIR}/unit.cpp"
)
set_target_properties(
    ${CATCH_OBJECT_NAME} PROPERTIES
        COMPILE_DEFINITIONS "$<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS>"
        COMPILE_OPTIONS "$<$<CXX_COMPILER_ID:MSVC>:/EHsc;$<$<CONFIG:Release>:/Od>>"
)
target_include_directories(
    ${CATCH_OBJECT_NAME} PUBLIC
        "${ARGPARSE_THIRD_PARTY_DIR}"
)

# test header
add_custom_target(
    ${LIBRARY_NAME}_test SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/argparse_test.hpp"
)

# tests with argparse declaration
file(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/unit-*.cpp")
foreach(file ${files})
    get_filename_component(file_basename ${file} NAME_WE)
    string(REGEX REPLACE "unit-([^$]+)" "test-\\1" testcase ${file_basename})

    add_executable(
        ${testcase}
            $<TARGET_OBJECTS:${LIBRARY_IMPL_NAME}>
            ${file}
    )
    target_link_libraries(${testcase} PRIVATE ${CATCH_OBJECT_NAME})
    add_test(${testcase} ${testcase})
endforeach()

# tests with argparse single-header
file(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/unit_single-*.cpp")
foreach(file ${files})
    get_filename_component(file_basename ${file} NAME_WE)
    string(REGEX REPLACE "unit_single-([^$]+)" "test-\\1" testcase ${file_basename})

    add_executable(${testcase} ${file})
    target_link_libraries(${testcase} PRIVATE ${CATCH_OBJECT_NAME})
    add_test(${testcase} ${testcase})
endforeach()
