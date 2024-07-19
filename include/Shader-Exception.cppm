module;

#include <format>
#include <glad/glad.h>
#include <string>

using namespace std::string_literals;
using std::string;

export module Shader:Exception;

import :Types;


export class ShaderException : public std::exception {
public:
    ShaderException(string infoLog);
    virtual ~ShaderException() = 0;
    const char* what() const override;

protected:
    string mInfoLog;
};

export class ShaderCompilationException : public ShaderException {
public:
    ShaderCompilationException(string infoLog, ShaderType shaderType);
    ShaderType getShaderType() const;

private:
    ShaderType mShaderType;
};

export class ShaderLinkingException : public ShaderException {
public:
    ShaderLinkingException(string infoLog);
};

