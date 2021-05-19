#include "Geometry.h"
#include <glm.hpp>
#include "Vertex.h"
#include <iostream>
#include <cstdlib>

Geometry::Geometry():
	m_Centroid(0,0,0),m_NormalCount(0),m_VertexCount(0)		//FrameRight(0),FrameTop(0),FrameFarview(0),FrameLeft(0),FrameDown(0),FrameNearview(0),
	
{

}

Geometry::~Geometry(void)
{
}


/// A file parser function that reads the vertex coordinates from the STL file & writes into vertices.
/// This parser uses ifstream to open the file, getline to read line & writes them into vertices.
/// @param m_vertices The vertices of the triangle elements.
/// @param filename: path of the modal file
/// @warning The vertices should have x,y,z component & must be a triangle element.
/// @see normalCalculation(),centriodCalculate().
void Geometry::stlpaser(const QString& filepath)
{
	std::string line, norm;
	double x = 0, y = 0, z = 0;
	std::ifstream ifs(filepath.toLatin1().data());
	if (ifs.is_open())
		std::cout << "File is Opened" << std::endl;

	while (!ifs.eof())
	{
		getline(ifs, line);

		if (line.find("vertex") != std::string::npos)
		{
			std::istringstream is(line);
			is >> norm >> x >> y >> z;
			m_Vertices.push_back(coordinates( x,y,z ));

			++m_VertexCount;
		}
	}

	normalCalculation();
	//centriodCalculate();
}
/// Calculates the surface normal of each triangular elements drawn using cross product.
///A = ai + bj + ck,
///B = xi + yj + zk
///
///A × B = (bz – cy)i – (az – cx)j + (ay – bx)k
///@param normal unit surface normal for each triangle element.

void Geometry::normalCalculation()
{
	for (unsigned int i = 0; i < m_Vertices.size(); )
	{
		double a[3] = { m_Vertices[i + 1].x - m_Vertices[i].x ,m_Vertices[i + 1].y - m_Vertices[i].y,m_Vertices[i + 1].z - m_Vertices[i].z };

		double b[3] = { m_Vertices[i + 2].x - m_Vertices[i].x ,m_Vertices[i + 2].y - m_Vertices[i].y,m_Vertices[i + 2].z - m_Vertices[i].z };

		double v[3] = { a[1] * b[2] - a[2] * b[1], -(a[0] * b[2] - a[2] * b[0]), a[0] * b[1] - a[1] * b[0] };

		double length = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
		for (int j = 0; j < 3; j++)
		{
			v[j] = v[j] / length;
		}

		m_Normal.push_back(coordinates(v[0], v[1], v[2]));
		m_NormalCount++;
		i = i + 3;
	}
}
/// Calculates the centroid of the model by calculating the average of all coordinates.
///@param centroid The centroid of the model.
///@see FrameSize(double&,double&,double&,double&,double&,double&).
void Geometry::centriodCalculate()
{
	double minX=0,minY=0,minZ=0,maxX=0,maxY=0,maxZ=0;
	maxX = m_Vertices[0].x; minX = m_Vertices[0].x;
	maxY = m_Vertices[0].y;minY = m_Vertices[0].y;
	maxZ = m_Vertices[0].z;minZ = m_Vertices[0].z;


	for (unsigned int i = 0; i < m_Vertices.size(); i++)
	{	m_Centroid.x += m_Vertices[i].x;
	if ( maxX < m_Vertices[i].x) maxX = m_Vertices[i].x;
	else if( minX > m_Vertices[i].x) minX = m_Vertices[i].x;
	m_Centroid.y += m_Vertices[i].y;
	if ( maxY < m_Vertices[i].y) maxY = m_Vertices[i].y;
	else if( minY > m_Vertices[i].y) minY =  m_Vertices[i].y;
	m_Centroid.z += m_Vertices[i].z;
	if ( maxZ < m_Vertices[i].z) maxZ = m_Vertices[i].z;
	else if( minZ > m_Vertices[i].z) minZ = m_Vertices[i].z;
	}
	m_Centroid.x = m_Centroid.x/m_VertexCount;
	m_Centroid.y = m_Centroid.y/m_VertexCount;
	m_Centroid.z = m_Centroid.z/m_VertexCount;
	//FrameSize(minX,minY,minZ,maxX,maxY,maxZ);
}
/*
/// Calculates a cuboid camera frame w.r.t the size of model.
///	@param maxX,maxY,maxZ maximum value of x,y,z coordinate.
///	@param minX,minY,minZ minimum value of x,y,z coordinate.
/// @param maxDist Distance from the centroid to form a cuboid frame.

void Geometry::FrameSize(double& minX,double& minY,double& minZ,double& maxX,double& maxY,double& maxZ)
{
	float Ḍist[]= {fabs((float)minX-(float)centroid.x),fabs((float)maxX-(float)centroid.x),
		fabs((float)minY-(float)centroid.y), fabs((float)maxY-(float)centroid.y)
		,fabs((float)minX-(float)centroid.z), fabs((float)maxZ-(float)centroid.z)};
	float maxDist= Ḍist[0];
	for(unsigned int k=1;k<6;k++)
	{if (maxDist<Ḍist[k]) maxDist=Ḍist[k];}
	int scale=1;
	FrameRight=maxDist+centroid.x+scale*maxDist; 
	FrameTop=maxDist+centroid.y+scale*maxDist;
	FrameFarview=maxDist+centroid.z+scale*maxDist;

	FrameLeft=centroid.x-maxDist -scale*maxDist;
	FrameDown=centroid.y-maxDist  -scale*maxDist;
	FrameNearview=centroid.z-maxDist -scale*maxDist;

}
*/
///This function organizes & stores the data of the modal in a batch of position,color& normal format into an object of Shapedata 
///@param m_vertices: The vertices of the triangle elements.
///@param m_Normal: The normal for each vertex
ShapeData Geometry::makeStl(const QString& filepath)
{
	
	stlpaser(filepath);		///Reading from the file
	ShapeData ret;
	
	ret.numVertices =m_Vertices.size();
	ret.vertices = new Vertex[ret.numVertices];
	for (unsigned int i = 0,n=0; i <ret.numVertices;n++)
	{
		ret.vertices[i].position=glm::vec3(m_Vertices[i].x,  m_Vertices[i].y, m_Vertices[i].z);			  // 0
		ret.vertices[i].color =glm::vec3(+1.0f, +1.0f, +0.0f);										 // Color
		ret.vertices[i].normal=glm::vec3(m_Normal[n].x,  m_Normal[n].y, m_Normal[n].z);					  // Normal
		ret.vertices[i+1].position=glm::vec3(m_Vertices[i+1].x,  m_Vertices[i+1].y, m_Vertices[i+1].z);	  // 1
		ret.vertices[i+1].color =glm::vec3(+1.0f, +1.0f, +0.0f);									 // Color
		ret.vertices[i+1].normal=glm::vec3(m_Normal[n].x,  m_Normal[n].y, m_Normal[n].z);					  // Normal
		ret.vertices[i+2].position=glm::vec3(m_Vertices[i+2].x,  m_Vertices[i+2].y, m_Vertices[i+2].z);	  // 2
		ret.vertices[i+2].color =glm::vec3(+1.0f, +1.0f, +0.0f);									  // Color
		ret.vertices[i+2].normal=glm::vec3(m_Normal[n].x,  m_Normal[n].y, m_Normal[n].z);					  // Normal
		i=i+3;																						  
	}																								  
																									  
	///Sending indexed data
	ret.numIndices = m_Vertices.size();
	ret.indices = new GLuint[ret.numIndices];
	for (unsigned int i = 0; i < ret.numIndices; i++)
	{ret.indices[i]=i;}
	return ret;
}
///This function organizes & stores the data of the modal in a batch of position,color& normal format into an object of Shapedata
ShapeData Geometry::makeArrow()
{
	ShapeData ret;
	Vertex stackVerts[] =
	{
		// Top side of arrow head
		glm::vec3(+0.00f, +0.25f, -0.25f),         // 0
		glm::vec3(+1.00f, +0.00f, +0.00f),		  // Color
		glm::vec3(+0.00f, +1.00f, +0.00f),         // Normal
		glm::vec3(+0.50f, +0.25f, -0.25f),         // 1
		glm::vec3(+1.00f, +0.00f, +0.00f),		  // Color
		glm::vec3(+0.00f, +1.00f, +0.00f),         // Normal
		glm::vec3(+0.00f, +0.25f, -1.00f),         // 2
		glm::vec3(+1.00f, +0.00f, +0.00f),		  // Color
		glm::vec3(+0.00f, +1.00f, +0.00f),         // Normal
		glm::vec3(-0.50f, +0.25f, -0.25f),         // 3
		glm::vec3(+1.00f, +0.00f, +0.00f),		  // Color
		glm::vec3(+0.00f, +1.00f, +0.00f),         // Normal
		// Bottom side of arrow head
		glm::vec3(+0.00f, -0.25f, -0.25f),         // 4
		glm::vec3(+0.00f, +0.00f, +1.00f),		  // Color
		glm::vec3(+0.00f, -1.00f, +0.00f),         // Normal
		glm::vec3(+0.50f, -0.25f, -0.25f),         // 5
		glm::vec3(+0.00f, +0.00f, +1.00f),		  // Color
		glm::vec3(+0.00f, -1.00f, +0.00f),         // Normal
		glm::vec3(+0.00f, -0.25f, -1.00f),         // 6
		glm::vec3(+0.00f, +0.00f, +1.00f),		  // Color
		glm::vec3(+0.00f, -1.00f, +0.00f),         // Normal
		glm::vec3(-0.50f, -0.25f, -0.25f),         // 7
		glm::vec3(+0.00f, +0.00f, +1.00f),		  // Color
		glm::vec3(+0.00f, -1.00f, +0.00f),         // Normal
		// Right side of arrow tip
		glm::vec3(+0.50f, +0.25f, -0.25f),         // 8
		glm::vec3(+0.60f, +1.00f, +0.00f),		  // Color
		glm::vec3(0.83205032f, 0.00f, -0.55470026f), // Normal
		glm::vec3(+0.00f, +0.25f, -1.00f),         // 9
		glm::vec3(+0.60f, +1.00f, +0.00f),		  // Color
		glm::vec3(0.83205032f, 0.00f, -0.55470026f), // Normal
		glm::vec3(+0.00f, -0.25f, -1.00f),         // 10
		glm::vec3(+0.60f, +1.00f, +0.00f),		  // Color
		glm::vec3(0.83205032f, 0.00f, -0.55470026f), // Normal
		glm::vec3(+0.50f, -0.25f, -0.25f),         // 11
		glm::vec3(+0.60f, +1.00f, +0.00f),		  // Color
		glm::vec3(0.83205032f, 0.00f, -0.55470026f), // Normal
		// Left side of arrow tip
		glm::vec3(+0.00f, +0.25f, -1.00f),         // 12
		glm::vec3(+0.00f, +1.00f, +0.00f),		  // Color
		glm::vec3(-0.55708605f, 0.00f, -0.37139067f), // Normal
		glm::vec3(-0.50f, +0.25f, -0.25f),         // 13
		glm::vec3(+0.00f, +1.00f, +0.00f),		  // Color
		glm::vec3(-0.55708605f, 0.00f, -0.37139067f), // Normal
		glm::vec3(+0.00f, -0.25f, -1.00f),         // 14
		glm::vec3(+0.00f, +1.00f, +0.00f),		  // Color
		glm::vec3(-0.55708605f, 0.00f, -0.37139067f), // Normal
		glm::vec3(-0.50f, -0.25f, -0.25f),         // 15
		glm::vec3(+0.00f, +1.00f, +0.00f),		  // Color
		glm::vec3(-0.55708605f, 0.00f, -0.37139067f), // Normal
		// Back side of arrow tip
		glm::vec3(-0.50f, +0.25f, -0.25f),         // 16
		glm::vec3(+0.50f, +0.50f, +0.50f),		  // Color
		glm::vec3(+0.00f, +0.00f, +1.00f),         // Normal
		glm::vec3(+0.50f, +0.25f, -0.25f),         // 17
		glm::vec3(+0.50f, +0.50f, +0.50f),		  // Color
		glm::vec3(+0.00f, +0.00f, +1.00f),         // Normal
		glm::vec3(-0.50f, -0.25f, -0.25f),         // 18
		glm::vec3(+0.50f, +0.50f, +0.50f),		  // Color
		glm::vec3(+0.00f, +0.00f, +1.00f),         // Normal
		glm::vec3(+0.50f, -0.25f, -0.25f),         // 19
		glm::vec3(+0.50f, +0.50f, +0.50f),		  // Color
		glm::vec3(+0.00f, +0.00f, +1.00f),         // Normal
		// Top side of back of arrow
		glm::vec3(+0.25f, +0.25f, -0.25f),         // 20
		glm::vec3(+1.00f, +0.00f, +0.00f),		  // Color
		glm::vec3(+0.00f, +1.00f, +0.00f),         // Normal
		glm::vec3(+0.25f, +0.25f, +1.00f),         // 21
		glm::vec3(+1.00f, +0.00f, +0.00f),		  // Color
		glm::vec3(+0.00f, +1.00f, +0.00f),         // Normal
		glm::vec3(-0.25f, +0.25f, +1.00f),         // 22
		glm::vec3(+1.00f, +0.00f, +0.00f),		  // Color
		glm::vec3(+0.00f, +1.00f, +0.00f),         // Normal
		glm::vec3(-0.25f, +0.25f, -0.25f),         // 23
		glm::vec3(+1.00f, +0.00f, +0.00f),		  // Color
		glm::vec3(+0.00f, +1.00f, +0.00f),         // Normal
		// Bottom side of back of arrow
		glm::vec3(+0.25f, -0.25f, -0.25f),         // 24
		glm::vec3(+0.00f, +0.00f, +1.00f),		  // Color
		glm::vec3(+0.00f, -1.00f, +0.00f),         // Normal
		glm::vec3(+0.25f, -0.25f, +1.00f),         // 25
		glm::vec3(+0.00f, +0.00f, +1.00f),		  // Color
		glm::vec3(+0.00f, -1.00f, +0.00f),         // Normal
		glm::vec3(-0.25f, -0.25f, +1.00f),         // 26
		glm::vec3(+0.00f, +0.00f, +1.00f),		  // Color
		glm::vec3(+0.00f, -1.00f, +0.00f),         // Normal
		glm::vec3(-0.25f, -0.25f, -0.25f),         // 27
		glm::vec3(+0.00f, +0.00f, +1.00f),		  // Color
		glm::vec3(+0.00f, -1.00f, +0.00f),         // Normal
		// Right side of back of arrow
		glm::vec3(+0.25f, +0.25f, -0.25f),         // 28
		glm::vec3(+0.60f, +1.00f, +0.00f),		  // Color
		glm::vec3(+1.00f, +0.00f, +0.00f),         // Normal
		glm::vec3(+0.25f, -0.25f, -0.25f),         // 29
		glm::vec3(+0.60f, +1.00f, +0.00f),		  // Color
		glm::vec3(+1.00f, +0.00f, +0.00f),         // Normal
		glm::vec3(+0.25f, -0.25f, +1.00f),         // 30
		glm::vec3(+0.60f, +1.00f, +0.00f),		  // Color
		glm::vec3(+1.00f, +0.00f, +0.00f),         // Normal
		glm::vec3(+0.25f, +0.25f, +1.00f),         // 31
		glm::vec3(+0.60f, +1.00f, +0.00f),		  // Color
		glm::vec3(+1.00f, +0.00f, +0.00f),         // Normal
		// Left side of back of arrow
		glm::vec3(-0.25f, +0.25f, -0.25f),         // 32
		glm::vec3(+0.00f, +1.00f, +0.00f),		  // Color
		glm::vec3(-1.00f, +0.00f, +0.00f),         // Normal
		glm::vec3(-0.25f, -0.25f, -0.25f),         // 33
		glm::vec3(+0.00f, +1.00f, +0.00f),		  // Color
		glm::vec3(-1.00f, +0.00f, +0.00f),         // Normal
		glm::vec3(-0.25f, -0.25f, +1.00f),         // 34
		glm::vec3(+0.00f, +1.00f, +0.00f),		  // Color
		glm::vec3(-1.00f, +0.00f, +0.00f),         // Normal
		glm::vec3(-0.25f, +0.25f, +1.00f),         // 35
		glm::vec3(+0.00f, +1.00f, +0.00f),		  // Color
		glm::vec3(-1.00f, +0.00f, +0.00f),         // Normal
		// Back side of back of arrow
		glm::vec3(-0.25f, +0.25f, +1.00f),         // 36
		glm::vec3(+0.50f, +0.50f, +0.50f),		  // Color
		glm::vec3(+0.00f, +0.00f, +1.00f),         // Normal
		glm::vec3(+0.25f, +0.25f, +1.00f),         // 37
		glm::vec3(+0.50f, +0.50f, +0.50f),		  // Color
		glm::vec3(+0.00f, +0.00f, +1.00f),         // Normal
		glm::vec3(-0.25f, -0.25f, +1.00f),         // 38
		glm::vec3(+0.50f, +0.50f, +0.50f),		  // Color
		glm::vec3(+0.00f, +0.00f, +1.00f),         // Normal
		glm::vec3(+0.25f, -0.25f, +1.00f),         // 39
		glm::vec3(+0.50f, +0.50f, +0.50f),		  // Color
		glm::vec3(+0.00f, +0.00f, +1.00f),         // Normal
	};

	GLushort stackIndices[] = {
		0, 1, 2, // Top
		0, 2, 3,
		4, 6, 5, // Bottom
		4, 7, 6,
		8, 10, 9, // Right side of arrow tip
		8, 11, 10,
		12, 15, 13, // Left side of arrow tip
		12, 14, 15,
		16, 19, 17, // Back side of arrow tip
		16, 18, 19,
		20, 22, 21, // Top side of back of arrow
		20, 23, 22,
		24, 25, 26, // Bottom side of back of arrow
		24, 26, 27,
		28, 30, 29, // Right side of back of arrow
		28, 31, 30,
		32, 33, 34, // Left side of back of arrow
		32, 34, 35,
		36, 39, 37, // Back side of back of arrow
		36, 38, 39,
	};

	ret.numVertices = sizeof(stackVerts) / sizeof(*stackVerts);
	ret.vertices = new Vertex[ret.numVertices];
	memcpy(ret.vertices, stackVerts, sizeof(stackVerts));

	ret.numIndices = sizeof(stackIndices) / sizeof(*stackIndices);
	ret.indices = new GLuint[ret.numIndices];
	memcpy(ret.indices, stackIndices, sizeof(stackIndices));
	return ret;
}
///Creates plane vertices for makePlane() function
ShapeData Geometry::makePlaneVerts(int dimensions)
{
	ShapeData ret;
	ret.numVertices = dimensions * dimensions;
	int half = dimensions / 2;
	ret.vertices = new Vertex[ret.numVertices];
	for (int i = 0; i < dimensions; i++)
	{
		for (int j = 0; j < dimensions; j++)
		{
			Vertex& thisVert = ret.vertices[i * dimensions + j];
			thisVert.position.x = j - half;
			thisVert.position.z = i - half;
			thisVert.position.y = 0;
			thisVert.normal = glm::vec3(0.0f, 1.0f, 0.0f);
			thisVert.color = randomColor();
		}
	}
	return ret;
}
///Creates indices for makePlane() function
ShapeData Geometry::makePlaneIndices(int dimensions)
{
	ShapeData ret;
	ret.numIndices = (dimensions - 1) * (dimensions - 1) * 2 * 3; // 2 triangles per square, 3 indices per triangle
	ret.indices = new GLuint[ret.numIndices];
	int runner = 0;
	for (int row = 0; row < dimensions - 1; row++)
	{
		for (int col = 0; col < dimensions - 1; col++)
		{
			ret.indices[runner++] = dimensions * row + col;
			ret.indices[runner++] = dimensions * row + col + dimensions;
			ret.indices[runner++] = dimensions * row + col + dimensions + 1;

			ret.indices[runner++] = dimensions * row + col;
			ret.indices[runner++] = dimensions * row + col + dimensions + 1;
			ret.indices[runner++] = dimensions * row + col + 1;
		}
	}
	assert(runner = ret.numIndices);
	return ret;
}

///This function organizes & stores the data of the modal in a batch of position,color& normal format into an object of Shapedata
ShapeData Geometry::makePlane(int dimensions)
{
	ShapeData ret = makePlaneVerts(dimensions);
	ShapeData ret2 = makePlaneIndices(dimensions);
	ret.numIndices = ret2.numIndices;
	ret.indices = ret2.indices;
	return ret;
}
///Creates random RGB color for the makePlane()
glm::vec3 Geometry::randomColor()
{
	glm::vec3 ret;
	ret.x = rand() / (float)RAND_MAX;
	ret.y = rand() / (float)RAND_MAX;
	ret.z = rand() / (float)RAND_MAX;
	return ret;
}
///Generates normal for each vertex
ShapeData Geometry::generateNormals(const ShapeData& data)
{
	ShapeData ret;
	ret.numVertices = data.numVertices * 2;
	ret.vertices = new Vertex[ret.numVertices];
	glm::vec3 white(1.0f, 1.0f, 1.0f);
	for (int i = 0; i < (int)data.numVertices; i++)
	{
		int vertIndex = i * 2;
		Vertex& v1 = ret.vertices[vertIndex];
		Vertex& v2 = ret.vertices[vertIndex + 1];
		const Vertex& sourceVertex = data.vertices[i];
		v1.position = sourceVertex.position;
		v2.position = sourceVertex.position + sourceVertex.normal;
		v1.color = v2.color = white;
	}
	ret.numIndices = ret.numVertices;
	ret.indices = new GLuint[ret.numIndices];
	for (int i = 0; i < (int)ret.numIndices; i++)
		ret.indices[i] = i;
	return ret;
}