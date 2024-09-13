module;

#include <string>

module Shader;

using std::string;

ShaderException::ShaderException(string infoLog)
    : mInfoLog{infoLog} {}

ShaderException::~ShaderException() {}

const char* ShaderException::what() const noexcept {
    return mInfoLog.c_str();
}

ShaderCompilationException::ShaderCompilationException(string infoLog, ShaderType shaderType)
    : ShaderException{infoLog}, mShaderType{shaderType} {}

ShaderType ShaderCompilationException::getShaderType() const {
    return mShaderType;
}

ShaderLinkingException::ShaderLinkingException(string infoLog)
    : ShaderException{infoLog} {}

