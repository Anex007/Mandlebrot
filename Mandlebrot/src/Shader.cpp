#include "Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
    : m_RendererId(0)
{
    std::ifstream vShaderFile(vertexPath);
    std::ifstream fShaderFile(fragmentPath);
    std::string vertexSrcStr;
    std::string fragmentSrcStr;

    // not good to do in constructor
    if (vShaderFile.fail() || fShaderFile.fail()) {
        std::cout << "Failed to open shaders" << std::endl;
        return;
    }

    std::stringstream vShaderStream, fShaderStream;
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    vertexSrcStr = vShaderStream.str();
    fragmentSrcStr = fShaderStream.str();

    vShaderFile.close();
    fShaderFile.close();

    const char* vertexSrc = vertexSrcStr.c_str();
    const char* fragmentSrc = fragmentSrcStr.c_str();

    unsigned int vertexShader, fragmentShader;
    char infoLog[512];
    int success;

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSrc, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Error compiling vertex shader: \n\t" << infoLog << std::endl;
        return;
    }
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSrc, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Error compiling fragment shader: \n\t" << infoLog << std::endl;
        return;
    }

    m_RendererId = glCreateProgram();
    glAttachShader(m_RendererId, vertexShader);
    glAttachShader(m_RendererId, fragmentShader);
    glLinkProgram(m_RendererId);
    glGetProgramiv(m_RendererId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_RendererId, 512, NULL, infoLog);
        std::cout << "Error Linking shaders: \n\t" << infoLog << std::endl;
        return;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

}

void Shader::Bind() const
{
    glUseProgram(m_RendererId);
}

void Shader::UploadUniform1d(const std::string& name, double v1) const
{
    int loc = glGetUniformLocation(m_RendererId, name.c_str());
    glUniform1d(loc, v1);
}

void Shader::UploadUniform2d(const std::string& name, double v1, double v2) const
{
    int loc = glGetUniformLocation(m_RendererId, name.c_str());
    glUniform2d(loc, v1, v2);
}

void Shader::UploadUniform3d(const std::string& name, double v1, double v2, double v3) const
{
    int loc = glGetUniformLocation(m_RendererId, name.c_str());
    glUniform3d(loc, v1, v2, v3);
}

void Shader::UploadUniform2f(const std::string& name, float v1, float v2) const
{
    int loc = glGetUniformLocation(m_RendererId, name.c_str());
    glUniform2f(loc, v1, v2);
}

void Shader::UploadUniformMat4(const std::string& name, const float* value) const
{
    int loc = glGetUniformLocation(m_RendererId, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, value);
}

void Shader::UploadUniform3f(const std::string& name, float v1, float v2, float v3) const
{
    // should probably bind before uploading.
    int uniformLoc = glGetUniformLocation(m_RendererId, name.c_str());
    glUniform3f(uniformLoc, v1, v2, v3);
}

void Shader::UploadUniform4f(const std::string& name, float v1, float v2, float v3, float v4) const
{
    // should probably bind before uploading.
    int uniformLoc = glGetUniformLocation(m_RendererId, name.c_str());
    glUniform4f(uniformLoc, v1, v2, v3, v4);
}

void Shader::UploadUniform1i(const std::string& name, int i) const
{
    int uniformLoc = glGetUniformLocation(m_RendererId, name.c_str());
    glUniform1i(uniformLoc, i);
}


void Shader::UploadUniform1f(const std::string& name, float v1) const
{
    int uniformLoc = glGetUniformLocation(m_RendererId, name.c_str());
    glUniform1f(uniformLoc, v1);
}

Shader::~Shader()
{
    glDeleteProgram(m_RendererId);
}