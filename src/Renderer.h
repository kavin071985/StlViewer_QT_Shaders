#pragma once
#include "GL/glew.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include <assert.h>

///Debugs the opengl function errors
#define ASSERT(x) if (!(x)) assert(false)

#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLCheckError(#x,__FILE__,__LINE__))

void GLClearError();
bool GLCheckError(const char* function, const char* file, int line);
///Binds the shader program,vertex array, index buffer and gives a draw call
///Clears the color & depth buffer bit
class Renderer
{
public:
	void Clear() const ; //Clears the color & depth buffer bit
	void Draw(const VertexArray *va, const IndexBuffer *ib, const Shader *shader)const ;
};
