cmake_minimum_required(VERSION 3.29)

# Referencing https://github.com/andersonfreitas/opengl-tutorial-org/blob/master/misc04_building_your_own_app/CMakeLists.txt

project(HelloOpenGL)

find_package(OpenGL REQUIRED)

# Set these variables if not using a preset
set(OPENGL_LIBS_INCLUDE_PATH "" CACHE PATH "Location of opengl headers")
set(GLFW_LIB_PATH "" CACHE PATH "Location of glfw lib")

if(NOT DEFINED OPENGL_LIBS_INCLUDE_PATH
    OR NOT IS_DIRECTORY "${OPENGL_LIBS_INCLUDE_PATH}"
    OR NOT DEFINED GLFW_LIB_PATH
    OR NOT EXISTS "${GLFW_LIB_PATH}"
)
    message(FATAL_ERROR
        "OPENGL_LIBS_INCLUDE_PATH and GLFW_LIB_PATH must be set:\n"
        "OPENGL_LIBS_INCLUDE_PATH: " ${OPENGL_LIBS_INCLUDE_PATH} "\n"
        "GLFW_LIB_PATH: " ${GLFW_LIB_PATH}
    )
endif()

add_library(Shader)
target_compile_features(Shader PUBLIC cxx_std_20)
target_sources(Shader
    PUBLIC
        FILE_SET CXX_MODULES FILES
            include/Shader.cppm
            include/Shader-Exception.cppm
            include/Shader-Types.cppm
        PRIVATE
            src/Shader-Exception.cpp
            src/Shader.cpp
)

set(SOURCE_FILES
    src/main.cpp
    src/glad.c
)
set(HEADER_FILES
    ""
)

include_directories(
    ${OPENGL_LIBS_INCLUDE_PATH}
)

add_executable(hello
    ${HEADER_FILES}
    ${SOURCE_FILES}
)
target_compile_features(hello PUBLIC cxx_std_20)
target_link_libraries(hello
    ${OPENGL_LIBRARY}
    ${GLFW_LIB_PATH}
    Shader
)
