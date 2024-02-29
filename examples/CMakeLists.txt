include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")

# examples with argparse declaration
file(GLOB examples "*/main.cpp")
foreach(path ${examples})
    get_filename_component(parent_dir ${path} DIRECTORY)
    get_filename_component(example ${parent_dir} NAME)
    file(GLOB files "${example}/*.c*" "${example}/*.h*")
    add_executable(${example} $<TARGET_OBJECTS:argparse_impl> ${files})
endforeach()
