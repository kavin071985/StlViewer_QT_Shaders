#include "VertexBuffer.h"
#include "Renderer.h"

///Generates Buffer ID, binds the id with the GL_ARRAY_BUFFER
///@param *data : pointer to the first address of the array of data that needs to be sent to VRam
///@param size : size in bytes for th whole array of data.
VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}
///Deletes the Vertex buffer
VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID));
}
///Binds the Vertex buffer
void VertexBuffer::Bind()
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}
///unbinds the Vertex buffer
void VertexBuffer::Unbind()
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
