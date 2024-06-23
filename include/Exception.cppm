module;

#include <format>
#include <glad/glad.h>
#include <string>

using namespace std::string_literals;
using std::string;

export module Exception;

export class ShaderException : public std::exception {
public:
    ShaderException(string infoLog);
    virtual ~ShaderException() = 0;
    const char* what() const override;

protected:
    string mInfoLog;
};

ShaderException::ShaderException(string infoLog)
    : mInfoLog{infoLog} {}

ShaderException::~ShaderException() {}

const char* ShaderException::what() const {
    return mInfoLog.c_str();
}

export class ShaderCompilationException : public ShaderException {
public:
    ShaderCompilationException(string infoLog, unsigned int shaderType);
    unsigned int getShaderType() const;

private:
    unsigned int mShaderType;
};

ShaderCompilationException::ShaderCompilationException(string infoLog, unsigned int shaderType)
    : ShaderException{infoLog}, mShaderType{shaderType} {}

unsigned int ShaderCompilationException::getShaderType() const {
    return mShaderType;
}

export class ShaderLinkingException : public ShaderException {
public:
    ShaderLinkingException(string infoLog);
};

ShaderLinkingException::ShaderLinkingException(string infoLog)
    : ShaderException{infoLog} {}

