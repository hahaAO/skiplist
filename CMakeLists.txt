cmake_minimum_required(VERSION 3.0.0)

SET(PROJECT_NAME skiplist)
project(
    ${PROJECT_NAME}
    VERSION 0.1.0
    LANGUAGES CXX
)

if(NOT CMAKE_CXX_STANDARD)
    SET(CMAKE_CXX_STANDARD 17)
    SET(CMAKE_CXX_STANDARD_REQUIRED ON)
    SET(CMAKE_CXX_EXTENSIONS OFF)
endif(NOT CMAKE_CXX_STANDARD)

SET(PROJECT_PUBLIC_INCLUDE_DIR "include/skiplist")
aux_source_directory(${PROJECT_SOURCE_DIR}/src project_source_file)
list(LENGTH project_source_file source_file_numbers)

if(${source_file_numbers} GREATER_EQUAL 1)
    add_library(${PROJECT_NAME} STATIC ${project_source_file})
    target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_PUBLIC_INCLUDE_DIR})
    install(
        TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}_TARGETS
        LIBRARY DESTINATION lib/${PROJECT_NAME}
        ARCHIVE DESTINATION lib/${PROJECT_NAME}
        RUNTIME DESTINATION bin/${PROJECT_NAME}
        INCLUDES DESTINATION include/${PROJECT_NAME}
    )
    install(EXPORT ${PROJECT_NAME}_TARGETS
        FILE ${PROJECT_NAME}-config.cmake
        DESTINATION ${PROJECT_BINARY_DIR}
    )
else()
    message(NOTICE "no have sources_file!")
endif()

file(GLOB EXPORT_HEADER_LIST ${PROJECT_PUBLIC_INCLUDE_DIR}/*.hpp ${PROJECT_PUBLIC_INCLUDE_DIR}/*.h)
install(FILES ${EXPORT_HEADER_LIST} DESTINATION include/${PROJECT_NAME})

add_executable(nono skiplist.cpp)
target_include_directories(nono PRIVATE include/skiplist)
target_link_libraries(nono PRIVATE -lpthread)
