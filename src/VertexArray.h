#pragma once

#include "VertexBuffer.h"
class VertexBufferLayout;
/** Creates a vertex Array for vertex buffer objects
*Suppose a vertex buffer layout with AddFloat(3),AddUnsignedInt(3),AddFloat(1) w.r.t a shape containing
* 3 float vertex position, 3 GLuint color, 1 float normal, the layout will have a stride of 7
*The following function creates a vertex attribute pointer for each offset of 0,3,3 & 
*sends data to different locations in the shader code 
*/
class VertexArray
{
    private:
        unsigned int m_RendererID;

    public:
        VertexArray();
        ~VertexArray();

		
        void AddBuffer(VertexBuffer& vb, const VertexBufferLayout& layout);
        void Bind() const;
        void Unbind() const;
};
