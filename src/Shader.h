#pragma once
#include "glm.hpp"
#include <string>
///Stores vertex & fragment shader code in different strings
struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};
///A Shader class to read a shader file & generate a shader programID containing vertex& fragment shaders.
class Shader
{
    private:
        unsigned int m_RendererID; ///ID of the shader program created
        std::string m_FilePath;		///file path to the shader text

    public:
        Shader(const std::string& filepath);
        ~Shader();
		inline GLuint getShaderID(){return m_RendererID;};
        void Bind() const;
        void Unbind() const;

        // Set uniforms
		void SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3);
		void SetUniform4fv(const std::string& name,const int& count, const glm::vec4& value);
		void SetUniformMatrix4fv(const std::string& name,const int& count,const bool& transPose, const glm::mat4& value);
		void SetUniform3fv(const std::string& name,const int& count, const glm::vec3& value);
        void SetUniform1f(const std::string& name, float value);

    private:
        int GetUniformLocation(const std::string& name);
        struct ShaderProgramSource ParseShader( std::string filepath);
        unsigned int CompileShader(unsigned int type, const std::string& source);
        unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

};
