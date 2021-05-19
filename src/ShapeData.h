#pragma once
#include <GL/glew.h>
#include "Vertex.h"
///Stores vertices,indices & their count
///@param *vertices: stores array of vertices
///@param *indices: stores array of indices
///@param numVertices : number of vertices in the array
///@param numIndices : number of indices in the array
struct ShapeData
{
	ShapeData(): vertices(0),numVertices(0),
		indices(0),numIndices(0){}
	Vertex* vertices;
	GLuint numVertices;
	GLuint* indices;
	GLuint index;
	GLuint numIndices;
	GLsizeiptr vertexBufferSize()const
	{
		return numVertices*sizeof(Vertex);
	}
	GLsizeiptr indexBufferSize()const
	{
		return numIndices*sizeof(GLuint);
	}

	void cleanup(){
		delete [] vertices;
		delete[] indices;
		numIndices = 0;
		numVertices=0;
	}
};