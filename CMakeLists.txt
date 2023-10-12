cmake_minimum_required(VERSION 3.16.0)

project(espert-sandbox VERSION 0.0.1 LANGUAGES C CXX)

set (CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME}
    main.cc
)

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${PROJECT_SOURCE_DIR}
)

add_subdirectory(espert-core)

target_link_libraries(${PROJECT_NAME} PUBLIC
    espert-core
)
