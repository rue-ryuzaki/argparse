include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")

include(example.cmake)

# examples with argparse declaration
file(GLOB examples "*/src/main.cpp")
foreach(path ${examples})
    get_filename_component(parent_dir ${path} DIRECTORY)
    get_filename_component(parent_dir ${parent_dir} DIRECTORY)
    get_filename_component(example ${parent_dir} NAME)
    add_example(${example})
endforeach()
