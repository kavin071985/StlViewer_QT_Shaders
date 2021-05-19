#pragma once

#include <vector>
#include <GL/glew.h>
#include "Renderer.h"

///the struct stores the type, count, normalized for each batch of vertices
///eg: i have a color with 3 float vertex(RGB) then type is GL_FLOAT,count is 3 & all vertices are normalised
struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int GetSizeOfType(unsigned int type)
    {
        switch (type)
        {
            case GL_FLOAT         : return sizeof(GLfloat);
            case GL_UNSIGNED_INT  : return sizeof(GLuint);
            case GL_UNSIGNED_BYTE : return sizeof(GLbyte);
        }
        ASSERT(false);
        return 0;
    }
};

/** Creates a layout for size & type of data that should be send to the vertexbuffer 
*suppose a shape contains 3 float vertexposition, 3 GLuint color, 1 float normal,
*then call AddFloat(3),AddUnsignedInt(3),AddFloat(1) so the layout will have a stride of 7
*.That's the layout of the vertex buffer block of memory
*/
class VertexBufferLayout
{
    private:
        unsigned int m_Stride;
        std::vector<VertexBufferElement> m_Elements;
		///
        void Push(unsigned int types, unsigned int count, unsigned char normalized)
        {
			VertexBufferElement a;
			a.type=types;a.count=count;a.normalized=normalized;
			m_Elements.push_back(VertexBufferElement(a));
            m_Stride += count * VertexBufferElement::GetSizeOfType(types);
        };

    public:
        VertexBufferLayout() :
            m_Stride(0) { }

        void AddFloat(unsigned int count)        { Push(GL_FLOAT, count, GL_FALSE);        }
        void AddUnsignedInt(unsigned int count)  { Push(GL_UNSIGNED_INT, count, GL_FALSE); }
        void AddUnsignedByte(unsigned int count) { Push(GL_UNSIGNED_BYTE, count, GL_TRUE); }

        inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; };
        inline unsigned int GetStride() const { return m_Stride; };
};
