#include "IndexBuffer.h"
#include "Renderer.h"
/** This class creates an index bufferID & stores index data & binds it with the GL_Elements_Array 
*@param *indices:points to the first memory of Array of indices
*@param count : total elements in the index array
*/
IndexBuffer::IndexBuffer(const unsigned int* indices, unsigned int count)
  :
    m_Count(count)
{
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    GLCall( glGenBuffers(1, &m_RendererID) );
    GLCall( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID) );
    GLCall( glBufferData(GL_ELEMENT_ARRAY_BUFFER, count* sizeof(unsigned int) , indices, GL_STATIC_DRAW) );
}
///Deletes the index buffer
IndexBuffer::~IndexBuffer()
{
    GLCall( glDeleteBuffers(1, &m_RendererID) );
}
///Binds the index buffer
void IndexBuffer::Bind() const
{
    GLCall( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID) );
}
///Unbinds the index buffer
void IndexBuffer::Unbind() const
{
    GLCall( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) );
}
