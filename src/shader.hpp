#pragma once
#include <string>
#include <unordered_map>

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader {
private:
    std::string m_FilePath;
    unsigned int m_RendererID;
    std::unordered_map<std::string,int> m_UniformLocationCache; 
public:
    Shader(const std::string& filepath);
    ~Shader();

    void bind();
    void unbind();

    //Set uniforms
    void setUniform4f(const std::string& name,float v1,float v2,float v3,float v4);
private:
    ShaderProgramSource parseShader(const std::string& filepath);
    unsigned int compileShader(unsigned int type, const std::string& source);
    unsigned int createShader(const std::string& vertexShader,const std::string& fragmentShader);
    unsigned int getUniformLocation(const std::string& name);
};