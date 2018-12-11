cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "1")

string(CONCAT CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}"
        ".${CPACK_PACKAGE_VERSION_MINOR}"
        ".${CPACK_PACKAGE_VERSION_PATCH}")

# project name mli - multilevel iterator
project(mli VERSION ${CPACK_PACKAGE_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-pedantic -Wall -Wextra -Wcast-align -Wcast-qual \
    -Wdisabled-optimization -Winit-self -Wno-noexcept-type \
    -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow \
    -Werror -Wno-unused ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -fprofile-arcs -ftest-coverage \
    ${CMAKE_CXX_FLAGS_DEBUG}")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release) # Set CMAKE_BUILD_TYPE to use in path PROJECT_BINARY_DIR
endif()

set(PROJECT_BINARY_DIR "${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")

add_subdirectory(libmli)
add_subdirectory(tests)