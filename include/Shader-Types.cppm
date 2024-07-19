module;

#include <glad/glad.h>

export module Shader:Types;

export enum class ShaderType {
    vertex = GL_VERTEX_SHADER,
    fragment = GL_FRAGMENT_SHADER
};
