#include "Renderer.h"
#include <iostream>

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}
///Checks opengl function error
///@param * function : opengl function name
///@param * file : compilation file tot which the function belongs to
///@param line	 : line number on on which the error has occurred  
bool GLCheckError(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{

		std::cout << "[OpenGL Error] (" << error << "):" << function << " " << file << "line:" << line << std::endl;
		switch (error) {
		case GL_INVALID_ENUM:
			std::cout << "GL_INVALID_ENUM : An unacceptable value is specified for an enumerated argument.";
			break;
		case GL_INVALID_VALUE:
			std::cout << "GL_INVALID_OPERATION : A numeric argument is out of range.";
			break;
		case GL_INVALID_OPERATION:
			std::cout << "GL_INVALID_OPERATION : The specified operation is not allowed in the current state.";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION : The frame buffer object is not complete.";
			break;
		case GL_OUT_OF_MEMORY:
			std::cout << "GL_OUT_OF_MEMORY : There is not enough memory left to execute the command.";
			break;
		case GL_STACK_UNDERFLOW:
			std::cout << "GL_STACK_UNDERFLOW : An attempt has been made to perform an operation that would cause an internal stack to underflow.";
			break;
		case GL_STACK_OVERFLOW:
			std::cout << "GL_STACK_OVERFLOW : An attempt has been made to perform an operation that would cause an internal stack to overflow.";
			break;
		default:
			std::cout << "Unrecognized error" << error;
		}
		std::cout << std::endl;
		return false;
	}
	return true;
}

///Clears the color & depth buffer bit
void Renderer::Clear()const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT); );
}
///Binds the shader program,vertex array, index buffer and gives a draw call
void Renderer::Draw(const VertexArray *va, const IndexBuffer *ib, const Shader *shader) const
{
	shader->Bind();
	va->Bind();
	ib->Bind();
	GLCall( glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, 0) );
}

