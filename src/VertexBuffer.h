#pragma once
///Generates Buffer ID, binds the id with the GL_ARRAY_BUFFER
///sends the data to the VRam
class VertexBuffer
{
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind();
	void Unbind();

private:
	unsigned int m_RendererID;
};
