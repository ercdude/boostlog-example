cmake_minimum_required(VERSION 3.8)

project(boost-log-testing)

set(CMAKE_CXX_STANDARD 11)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(boost-log-testing main.cpp log.cpp)
target_link_libraries(boost-log-testing ${CONAN_LIBS})
