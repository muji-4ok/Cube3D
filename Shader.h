#pragma once
#include <glad/glad.h>
#include <iostream>


class Shader
{
public:
    Shader(GLenum shaderType, std::string filename);

    Shader(Shader&& s);
    Shader& operator= (Shader&& s);

    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator= (const Shader&) = delete;

    unsigned int getID() const;

private:
    unsigned int ID;
    GLenum shaderType;
};


class ShaderProgram
{
public:
    ShaderProgram() : ID(0) {}
    
    ShaderProgram(const Shader &vertexShader, const Shader &fragmentShader);

    ShaderProgram(ShaderProgram&& sp);
    ShaderProgram& operator= (ShaderProgram&& sp);

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator= (const ShaderProgram&) = delete;

    ~ShaderProgram();

    void use();
    static void disable();
    void setUniform1f(const std::string &name, const float &value);

private:
    unsigned int ID;

    int getUniformLocation(const std::string &name) const;
};
