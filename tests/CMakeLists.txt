if (NOT EXTERNALS_DIR)
    set(EXTERNALS_DIR ${CMAKE_SOURCE_DIR}/externals)
endif()

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

option(INSTALL_GMOCK "Install Googletest's GMock" OFF)
option(INSTALL_GTEST "Install Googletest's GTest" OFF)

add_subdirectory(${EXTERNALS_DIR}/googletest/${googletest_SOURCE_DIR}
        ${PROJECT_BINARY_DIR}/googletest/${googletest_BINARY_DIR})

set(utilsDir ${CMAKE_HOME_DIRECTORY}/tests/utils)
#list(APPEND utilsList "${utilsDir}/test_classes.hpp")

function(AddTest file_name)
    set(executableName test_${file_name})
    message("executableName is ${executableName}")

    file(GLOB thisSourceName src/${executableName}.cpp)

    add_executable(${executableName} ${thisSourceName})

    #target_sources(${executableName} PRIVATE ${utilsList})
    target_include_directories(${executableName} PUBLIC ${utilsDir})

    target_link_libraries(${executableName} ${PROJECT_NAME} gtest_main gmock pthread)
    set_target_properties(${executableName} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin/tests/")
    add_test(NAME ${executableName}
            WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/bin/tests/"
            COMMAND ${executableName})
endfunction()

# add new tests here
AddTest(multilevel_iterator)
AddTest(multilevel_container)