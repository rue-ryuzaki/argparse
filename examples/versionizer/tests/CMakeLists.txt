include_directories(${ARGPARSE_THIRD_PARTY_DIR})
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../src")

if (NOT TARGET catch_main)
    add_library(catch_main OBJECT "unit.cpp")
    set_target_properties(catch_main PROPERTIES
        COMPILE_DEFINITIONS "$<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS>"
        COMPILE_OPTIONS "$<$<CXX_COMPILER_ID:MSVC>:/EHsc;$<$<CONFIG:Release>:/Od>>"
    )
endif()

file(GLOB files "unit-*.cpp")

foreach(file ${files})
    get_filename_component(file_basename ${file} NAME_WE)
    string(REGEX REPLACE "unit-([^$]+)" "test-\\1" testcase ${file_basename})

    add_executable(${testcase} $<TARGET_OBJECTS:catch_main> ${file})
endforeach()
