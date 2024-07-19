module;

#include <array>
#include <glad/glad.h>

export module Shader;

export import :Types;
export import :Exception;

class Shader {
public:
    Shader(unsigned int id);
    ~Shader();

    // Prevent copies because we should only tell OpenGL to delete the shader once
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    unsigned int getId() const;

    static constexpr size_t LOG_LENGTH = 512;

protected:
    void pCheckCompileStatus(ShaderType shaderType);

    unsigned int mId;
};

class VertexShader : public Shader {
public:
    VertexShader();
};

class FragmentShader : public Shader {
public:
    FragmentShader(std::array<float, 4> color);
    FragmentShader(float r, float g, float b, float a);

private:
    void pInitialize();

    std::array<float, 4> mColor;
};

export class ShaderProgram {
public:
    ShaderProgram(std::array<float, 4> color);
    ShaderProgram(float r, float g, float b, float a);

    unsigned int getId() const;

    static constexpr size_t LOG_LENGTH = 512;

private:
    void pInitialize(std::array<float, 4> color);

    unsigned int mId;
};
