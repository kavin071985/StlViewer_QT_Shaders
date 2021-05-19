#pragma once
/** This class creates an index bufferID & stores index data & binds it with the GL_Elements_Array 
*@param m_RendererID: ID of the index buffer
*@param m_Count: number index buffer elements
*/
class IndexBuffer
{
 public:
     IndexBuffer(const unsigned int* indices, unsigned int count);
     ~IndexBuffer();

     void Bind() const;
     void Unbind() const;

     inline unsigned int GetCount() const { return m_Count; }

 private:
     unsigned int m_RendererID;
     unsigned int m_Count;
};
