#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"
///Generates a vertex array ID
VertexArray::VertexArray()
{
    GLCall( glGenVertexArrays(1, &m_RendererID) );
}
///Deletes the vertex array ID
VertexArray::~VertexArray()
{
    GLCall( glDeleteVertexArrays(1, &m_RendererID) );
}
/**Suppose a vertex buffer layout with AddFloat(3),AddUnsignedInt(3),AddFloat(1) w.r.t a shape containing
* 3 float vertex position, 3 GLuint color, 1 float normal, the layout will have a stride of 7
*The following function binds the vertex array & then vertex buffer objects ,creates a vertex attribute pointer
*for each offset of 0,3,3 & that sends data to different locations in the shader code 
*/
void VertexArray::AddBuffer( VertexBuffer& vb, const VertexBufferLayout& layout)
{
    Bind();
    vb.Bind();
    const std::vector<VertexBufferElement> elements = layout.GetElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size() ; i++)
    {
        const VertexBufferElement element = elements[i];
        GLCall( glEnableVertexAttribArray(i) );
        GLCall( glVertexAttribPointer(i, element.count, element.type, element.normalized,
                                      layout.GetStride(), (const void*)offset) );
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}
///Binds the Vertex array 
void VertexArray::Bind() const
{
    GLCall( glBindVertexArray(m_RendererID) );
}
///Unbinds the Vertex array 
void VertexArray::Unbind() const
{
    GLCall( glBindVertexArray(0) );
};
