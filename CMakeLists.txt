project("ArgumentParser")
cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)
include_directories(third_party)

enable_testing()
add_subdirectory(test)
