include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../third_party")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")

add_library(catch_main OBJECT "unit.cpp")
add_library(argparse_impl OBJECT "argparse_impl.cpp")

set_target_properties(catch_main PROPERTIES
    COMPILE_DEFINITIONS "$<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS>"
    COMPILE_OPTIONS "$<$<CXX_COMPILER_ID:MSVC>:/EHsc;$<$<CONFIG:Release>:/Od>>"
)

set_target_properties(argparse_impl PROPERTIES
    COMPILE_DEFINITIONS "$<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS>"
    COMPILE_OPTIONS "$<$<CXX_COMPILER_ID:MSVC>:/EHsc;$<$<CONFIG:Release>:/Od>>"
)

file(GLOB files "unit-*.cpp")

foreach(file ${files})
    get_filename_component(file_basename ${file} NAME_WE)
    string(REGEX REPLACE "unit-([^$]+)" "test-\\1" testcase ${file_basename})

    add_executable(${testcase} $<TARGET_OBJECTS:catch_main> $<TARGET_OBJECTS:argparse_impl> ${file})
    add_test(${testcase} ${testcase})
endforeach()
