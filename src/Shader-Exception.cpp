module;

#include <string>
using std::string;

module Shader;

ShaderException::ShaderException(string infoLog)
    : mInfoLog{infoLog} {}

ShaderException::~ShaderException() {}

const char* ShaderException::what() const {
    return mInfoLog.c_str();
}

ShaderCompilationException::ShaderCompilationException(string infoLog, ShaderType shaderType)
    : ShaderException{infoLog}, mShaderType{shaderType} {}

ShaderType ShaderCompilationException::getShaderType() const {
    return mShaderType;
}

ShaderLinkingException::ShaderLinkingException(string infoLog)
    : ShaderException{infoLog} {}

