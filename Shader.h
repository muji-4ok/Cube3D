#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <iostream>
#include <vector>


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
    void setUniformVector3fv(const std::string &name, const std::vector<float> &vecs);
    void setUniform1fv(const std::string &name, const std::vector<float> &vals);
    void setUniform1f(const std::string &name, const float &value);
    void setUniform1i(const std::string &name, const int &value);
    void setUniformMatrix4fv(const std::string &name, const glm::mat4 &mat4);
    void setUniformMatrix3fv(const std::string &name, const glm::mat3 &mat3);

private:
    unsigned int ID;

    int getUniformLocation(const std::string &name) const;
};
