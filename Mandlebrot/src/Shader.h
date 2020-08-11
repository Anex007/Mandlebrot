#pragma once

#include <glad/glad.h>

#include <string>

class Shader
{
public:
    Shader() : m_RendererId(0) {}
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    void Bind() const;
    inline unsigned int GetProgram() const { return m_RendererId; }
    void UploadUniform1d(const std::string& name, double v1) const;
    void UploadUniform2d(const std::string& name, double v1, double v2) const;
    void UploadUniform3d(const std::string& name, double v1, double v2, double v3) const;
    void UploadUniformMat4(const std::string& name, const float* value) const;
    void UploadUniform2f(const std::string& name, float v1, float v2) const;
    void UploadUniform3f(const std::string& name, float v1, float v2, float v3) const;
    void UploadUniform4f(const std::string& name, float v1, float v2, float v3, float v4) const;
    void UploadUniform1i(const std::string& name, int i) const;
    void UploadUniform1f(const std::string& name, float v1) const;
    ~Shader();
private:
    unsigned int m_RendererId;
};