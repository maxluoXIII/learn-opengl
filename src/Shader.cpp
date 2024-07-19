module;

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <array>
#include <format>
#include <iostream>

module Shader;

using std::string;

constexpr const char* VERTEX_SHADER_SOURCE_TEMPLATE = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main() {\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

constexpr const char* FRAGMENT_SHADER_SOURCE_TEMPLATE =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() {{\n"
    "   FragColor = vec4({}, {}, {}, {});\n"
    "}}\0";

Shader::Shader(unsigned int id)
    : mId{id} {}

Shader::~Shader() {
    glDeleteShader(mId);
}

unsigned int Shader::getId() const {
    return mId;
}

void Shader::pCheckCompileStatus(ShaderType shaderType) {
    int success;
    string infoLog;
    infoLog.reserve(LOG_LENGTH);
    glGetShaderiv(mId, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(mId, LOG_LENGTH, NULL, infoLog.data());
        throw ShaderCompilationException{infoLog, shaderType};
    }
}

VertexShader::VertexShader()
    : Shader{glCreateShader(GL_VERTEX_SHADER)} {

    // Compile the shader
    glShaderSource(mId, 1, &VERTEX_SHADER_SOURCE_TEMPLATE, NULL);
    glCompileShader(mId);

    // Check whether compilation was successful
    pCheckCompileStatus(ShaderType::vertex);
}

FragmentShader::FragmentShader(std::array<float, 4> color)
    : Shader{glCreateShader(GL_FRAGMENT_SHADER)}, mColor{color} {

    pInitialize();
}

FragmentShader::FragmentShader(float r, float g, float b, float a)
    : Shader{glCreateShader(GL_FRAGMENT_SHADER)}, mColor{r, g, b, a} {

    pInitialize();
}

void FragmentShader::pInitialize() {
    // Compile the shader

    string shaderSource = std::format(
            FRAGMENT_SHADER_SOURCE_TEMPLATE, 
            mColor[0], mColor[1], mColor[2], mColor[3]);
    auto shaderSourceCStr = shaderSource.c_str();
    glShaderSource(mId, 1, 
            &shaderSourceCStr, 
            NULL);
    glCompileShader(mId);

    // Check whether compilation was successful
    pCheckCompileStatus(ShaderType::fragment);
}

ShaderProgram::ShaderProgram(std::array<float, 4> color)
    : mId{glCreateProgram()} {

    pInitialize(color);
}

ShaderProgram::ShaderProgram(float r, float g, float b, float a)
    : mId{glCreateProgram()} {

    pInitialize({r, g, b, a});
}

unsigned int ShaderProgram::getId() const {
    return mId;
}

void ShaderProgram::pInitialize(std::array<float, 4> color) {
    // Compile shaders
    VertexShader vertexShader;
    FragmentShader fragmentShader{color};

    // Link the compiled shaders into a shader program
    glAttachShader(mId, vertexShader.getId());
    glAttachShader(mId, fragmentShader.getId());
    glLinkProgram(mId);

    // Check whether linking was successful
    int success;
    char infoLog[LOG_LENGTH];
    glGetProgramiv(mId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(mId, LOG_LENGTH, NULL, infoLog);
        throw ShaderLinkingException{infoLog};
    }
}
