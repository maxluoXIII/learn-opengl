cmake_minimum_required(VERSION 3.29)

# Referencing https://github.com/andersonfreitas/opengl-tutorial-org/blob/master/misc04_building_your_own_app/CMakeLists.txt

project(HelloOpenGL)

# We are using C++20 modules
set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

set(OPENGL_INCLUDE_PATH "C:\\Users\\gutha\\libs\\OpenGL\\include" CACHE PATH "Location of opengl headers")
set(GLFW_LIB_PATH "C:\\Users\\gutha\\libs\\OpenGL\\lib\\glfw3.lib" CACHE PATH "Location of glfw lib")

add_library(Exception)
target_sources(Exception
    PUBLIC
        FILE_SET CXX_MODULES FILES
            include/Exception.cppm
)

set(SOURCE_FILES
    src/main.cpp
    src/glad.c
)
set(HEADER_FILES "")

include_directories(
    ${OPENGL_INCLUDE_PATH}
)

add_executable(hello
    ${HEADER_FILES}
    ${SOURCE_FILES}
)

target_link_libraries(hello
    ${OPENGL_LIBRARY}
    ${GLFW_LIB_PATH}
    Exception
)
