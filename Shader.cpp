#include "Shader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>

Shader::Shader(GLenum shaderType, std::string filename) : shaderType(shaderType)
{
    std::ifstream source(filename);
    std::stringstream textStream;

    if (source.is_open())
    {
        std::string line;

        while (std::getline(source, line))
            textStream << line << "\n";
    }
    else
    {
        std::cerr << "Couldn't open shader file " << filename << " \n";
        exit(1);
    }

    std::string stringText = textStream.str();
    const char *c_strText = stringText.c_str();

    ID = glCreateShader(shaderType);
    glShaderSource(ID, 1, &c_strText, nullptr);
    glCompileShader(ID);

    int success;
    char infoLog[512];
    glGetShaderiv(ID, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(ID, 512, nullptr, infoLog);
        std::cout << "Error compiling shader " << filename << "\n" << infoLog << "\n";
    }
}

Shader::Shader(Shader&& s)
{
    ID = s.ID;
    shaderType = s.shaderType;

    s.ID = 0;
}

Shader& Shader::operator=(Shader&& s)
{
    if (this == &s)
        return *this;

    ID = s.ID;
    shaderType = s.shaderType;

    s.ID = 0;

    return *this;
}

unsigned int Shader::getID() const
{
    return ID;
}

Shader::~Shader()
{
    glDeleteShader(ID);
}

ShaderProgram::ShaderProgram(const Shader &vertexShader, const Shader &fragmentShader)
{
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader.getID());
    glAttachShader(ID, fragmentShader.getID());
    glLinkProgram(ID);

    int success;
    char infoLog[512];

    glGetProgramiv(ID, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        std::cout << "Error linking program\n" << infoLog << std::endl;
    }
}

ShaderProgram::ShaderProgram(ShaderProgram&& sp)
{
    ID = sp.ID;
    sp.ID = 0;
}

ShaderProgram & ShaderProgram::operator=(ShaderProgram && sp)
{
    if (this == &sp)
        return *this;

    ID = sp.ID;
    sp.ID = 0;

    return *this;
}

void ShaderProgram::use()
{
    glUseProgram(ID);
}

void ShaderProgram::disable()
{
    glUseProgram(0);
}

void ShaderProgram::setUniform1f(const std::string &name, const float &value)
{
    int uniformLocation = getUniformLocation(name);
    glUniform1f(uniformLocation, value);
}

int ShaderProgram::getUniformLocation(const std::string &name) const
{
    static std::map<std::string, int> cache;

    auto elem = cache.find(name);

    if (elem != cache.end())
    {
        return elem->second;
    }
    else
    {
        int loc = glGetUniformLocation(ID, name.c_str());
        cache[name] = loc;

        return loc;
    }
}

ShaderProgram::~ShaderProgram()
{
    ShaderProgram::disable();
    glDeleteProgram(ID);
}