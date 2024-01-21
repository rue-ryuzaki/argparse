include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")

file(GLOB files "example-*.cpp")
foreach(file ${files})
    get_filename_component(file_basename ${file} NAME_WE)
    string(REGEX REPLACE "example-([^$]+)" "\\1" example ${file_basename})

    add_executable(${example} $<TARGET_OBJECTS:argparse_impl> ${file})
endforeach()
