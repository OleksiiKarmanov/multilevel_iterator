list(APPEND ${PROJECT_NAME}_PUBLIC_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/deref.hpp")
list(APPEND ${PROJECT_NAME}_PUBLIC_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/multilevel_iterator.hpp")
list(APPEND ${PROJECT_NAME}_PUBLIC_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/multilevel_container.hpp")
list(APPEND ${PROJECT_NAME}_PUBLIC_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/traits.hpp")

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE include)

install(FILES ${${PROJECT_NAME}_PUBLIC_HEADERS} DESTINATION "include")