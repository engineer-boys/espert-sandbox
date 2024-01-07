cmake_minimum_required(VERSION 3.16.0)

project(espert-sandbox VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cc)

add_executable(${PROJECT_NAME}
    ${SOURCES}
)

if(WIN32)
    add_compile_definitions(_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/ignore:4099")
endif()

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/espert-core/src
    ${PROJECT_SOURCE_DIR}/espert-core/src/Core
)

include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(espert-core)

target_link_libraries(${PROJECT_NAME} PUBLIC
        espert-core
)


############## Build shaders #######################
# get all .vert and .frag files in shaders directory
file(GLOB_RECURSE GLSL_SOURCE_FILES
        "${PROJECT_SOURCE_DIR}/resources/Shaders/*.frag"
        "${PROJECT_SOURCE_DIR}/resources/Shaders/*.vert"
        )

set(GLSLANG ${CMAKE_CURRENT_BINARY_DIR}/espert-core/bin/glslang)
if (WIN32)
        set(GLSLANG ${GLSLANG}.exe)
endif()

# run glslang compiler for every shader source file
foreach (GLSL ${GLSL_SOURCE_FILES})
    set(SPIRV "${GLSL}.spv")
    add_custom_command(
            OUTPUT ${SPIRV}
            COMMAND ${GLSLANG} -V ${GLSL} -o ${SPIRV}
            DEPENDS ${GLSL}
    )
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach (GLSL)

add_custom_target(
        sandbox_shaders
        DEPENDS ${SPIRV_BINARY_FILES}
)
add_dependencies(${PROJECT_NAME} sandbox_shaders)
add_dependencies(sandbox_shaders espert-core)
