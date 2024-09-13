module;

#include <glad/glad.h>
#include <string>

export module Shader:Exception;

import :Types;

using std::string;

export class ShaderException : public std::exception {
public:
    ShaderException(string infoLog);
    virtual ~ShaderException() = 0;
    const char* what() const noexcept override;

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

