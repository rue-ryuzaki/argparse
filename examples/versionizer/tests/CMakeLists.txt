file(GLOB files "unit-*.cpp")

foreach(file ${files})
    get_filename_component(file_basename ${file} NAME_WE)
    string(REGEX REPLACE "unit-([^$]+)" "test-\\1" testcase ${file_basename})

    add_executable(${testcase} "${CMAKE_CURRENT_SOURCE_DIR}/unit.cpp" ${file})
    target_include_directories(
        ${testcase} PRIVATE
            "${ARGPARSE_THIRD_PARTY_DIR}"
            "${CMAKE_CURRENT_SOURCE_DIR}/../include"
            "${CMAKE_CURRENT_SOURCE_DIR}/../src"
    )
endforeach()
