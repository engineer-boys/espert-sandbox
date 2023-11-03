cmake_minimum_required(VERSION 3.16.0)

project(espert-sandbox VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cc)

add_executable(${PROJECT_NAME}
    ${SOURCES}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/espert-core/src
)

include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(espert-core)

target_link_libraries(${PROJECT_NAME} PUBLIC
        espert-core
        )

############## Build shaders #######################
# get all .vert and .frag files in shaders directory
file(GLOB_RECURSE GLSL_SOURCE_FILES
        "${PROJECT_SOURCE_DIR}/shaders/*.frag"
        "${PROJECT_SOURCE_DIR}/shaders/*.vert"
        )

# run glslang compiler for every shader source file
foreach (GLSL ${GLSL_SOURCE_FILES})
    get_filename_component(FILE_NAME ${GLSL} NAME)
    get_filename_component(DIR_PATH ${GLSL} DIRECTORY)
    get_filename_component(FILE_DIR ${DIR_PATH} NAME)
    set(SPIRV "${PROJECT_SOURCE_DIR}/shaders/${FILE_DIR}/${FILE_NAME}.spv")
    add_custom_command(
            OUTPUT ${SPIRV}
            COMMAND $<TARGET_FILE:glslang-standalone> -V ${GLSL} -o ${SPIRV}
            DEPENDS ${GLSL}
    )
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach (GLSL)

add_custom_target(
        sandbox_shaders
        DEPENDS ${SPIRV_BINARY_FILES}
)
add_dependencies(${PROJECT_NAME} sandbox_shaders)
