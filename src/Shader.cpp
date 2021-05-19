#include "Renderer.h"
#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
///Reads the filepath of the shader file using ParseShader() , 
///prints the shader code on console from file & creates a shader program ID ,m_RendererID
///calls the shader program
Shader::Shader(const std::string& filepath)
    : m_FilePath(filepath), m_RendererID(0)
{
    ShaderProgramSource source = ParseShader(filepath);

    std::cout << "VERTEX" << std::endl << source.VertexSource << std::endl;
    std::cout << "FRAGMENT" << std::endl << source.FragmentSource << std::endl;

    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);

    GLCall( glUseProgram(m_RendererID) );
}
/// After the shader goes out of scope it automatically deletes the shader program
Shader::~Shader()
{
    GLCall( glDeleteProgram(m_RendererID) );
}

/// Enables the shader program when called
void Shader::Bind() const
{
    GLCall( glUseProgram(m_RendererID) );
}

/// Disables the shader program when called
void Shader::Unbind() const
{
    GLCall( glUseProgram(0) );
}

/// Searches through the shader code to find an uniform
///@param name : name of the uniform
///@returns int : ID of the uniform
int Shader::GetUniformLocation(const std::string& name)
{
  

    GLCall( int location = glGetUniformLocation(m_RendererID, name.c_str()) );
    if (location == -1)
        std::cout << "No active uniform variable with name " << name << " found" << std::endl;

    return location;
}
/// Sends uniform data to single float uniform variable.
///@param name : Name of the uniform in shader code
///@param value : Value that should be sent to the uniform in the shader

void Shader::SetUniform1f(const std::string& name, float value)
{
    GLCall( glUniform1f(GetUniformLocation(name), value) );
}

/// Sends uniform data to an uniform variable which takes 4 floats.
///@param name : Name of the uniform in shader code
///@param f0,f1,f2,f3 : Float values that should be sent to the uniform in the shader
void Shader::SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3)
{
	GLCall( glUniform4f(GetUniformLocation(name), f0, f1, f2, f3) );
}

/// Sends uniform data to an uniform variable which takes array of 4 floats.
///@param name : Name of the uniform in shader code
///@param value : Float array that should be sent to the uniform in the shader
///@param count : Number of times the Float array should be sent to the uniform in the shader
void Shader::SetUniform4fv(const std::string& name,const int& count,const glm::vec4& value)
{
	GLCall( glUniform4fv(GetUniformLocation(name),count, &value[0]));
	
}

/// Sends uniform data to an uniform variable which takes a Mat4x4 matrix .
///@param name : Name of the uniform in shader code
///@param value : Mat4x4 matrix that should be sent to the uniform in the shader
///@param count : Number of times the Mat4x4 matrix should be sent to the uniform in the shader
///@param transpose : Whether to transpose the matrix or not

void Shader::SetUniformMatrix4fv(const std::string& name,const int& count,const bool& transPose, const glm::mat4& value)
{
	GLCall( glUniformMatrix4fv(GetUniformLocation(name),count,transPose, &value[0][0]));
}
/// Sends uniform data to an uniform variable which takes array of 3 floats.
///@param name : Name of the uniform in shader code
///@param value : Float array that should be sent to the uniform in the shader
///@param count : Number of times the Float array should be sent to the uniform in the shader

void Shader::SetUniform3fv(const std::string& name,const int& count, const glm::vec3& value)
{
	
	GLCall( glUniform3fv(GetUniformLocation(name),count, &value[0]));
}

enum ShaderType
{
    NONE = -1, VERTEX = 0, FRAGMENT = 1
};

///Reads the file containing shader code, extracts the files into vertex & fragmentShader strings
///@param filepath : path where the shader file is stored
///@ returns ShaderProgramSource
struct ShaderProgramSource Shader::ParseShader( std::string filepath)
{
	const char* file= filepath.c_str();
    std::ifstream stream(file);
    std::string line;
    std::stringstream ss[2];
    ShaderType type = NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    struct ShaderProgramSource sps = { ss[0].str(), ss[1].str() };
    return sps;
}
///Converts the source shader string into vertex or fragment shader
///@param type : vertex shader or fragment shader 
///@param source: a string for vertex shader or fragment shader 
///@returns unsigned int id of the vertex/fragment shader
unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    GLCall( unsigned int id = glCreateShader(type) );
    const char* src = source.c_str();
    GLCall( glShaderSource(id, 1, &src, 0) );
    GLCall( glCompileShader(id) );

    // Error handling
    int result;
    GLCall( glGetShaderiv(id, GL_COMPILE_STATUS, &result) );
    std::cout << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader compile status: " << result << std::endl;
    if ( result == GL_FALSE )
    {
        int length;
        GLCall( glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length) );
        char* message = (char*) alloca(length * sizeof(char));
        GLCall( glGetShaderInfoLog(id, length, &length, message) );
        std::cout 
            << "Failed to compile "
            << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << "shader"
            << std::endl;
        std::cout << message << std::endl;
        GLCall( glDeleteShader(id) );
        return 0;
    }

    return id;
}
///creates a shader program by combining vertex & fragment shaders into a single program,links & validates the program
///@returns unsigned int program ID
unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    // create a shader program
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall( glAttachShader(program, vs) );
    GLCall( glAttachShader(program, fs) );

    GLCall( glLinkProgram(program) );

    GLint program_linked;

    GLCall( glGetProgramiv(program, GL_LINK_STATUS, &program_linked) );
    std::cout << "Program link status: " << program_linked << std::endl;
    if (program_linked != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        GLCall( glGetProgramInfoLog(program, 1024, &log_length, message) );
        std::cout << "Failed to link program" << std::endl;
        std::cout << message << std::endl;
    }

    GLCall( glValidateProgram(program) );

    GLCall( glDeleteShader(vs) );
    GLCall( glDeleteShader(fs) );

    return program;
}
