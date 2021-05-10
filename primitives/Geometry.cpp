#include "Geometry.h"
#include <glm.hpp>
#include "primitives/Vertex.h"
#include <iostream>
#include <cstdlib>

Geometry::Geometry():
	FrameRight(0),FrameTop(0),FrameFarview(0),FrameLeft(0),FrameDown(0),FrameNearview(0),centroid(0,0,0)
{

}

Geometry::~Geometry(void)
{
}


/// A file parser function that reads the vertex coordinates from the STL file & writes into vertices.
/// This parser uses ifstream to open the file, getline to read line & writes them into vertices.
/// @param vertices The vertices of the triangle elements.
/// @warning The vertices should have x,y,z component & must be a triangle element.
/// @see normalCalculation(),centriodCalculate().
void Geometry::stlpaser()
{
	static int nl=0;
	static int vx=0;

	std::string line, norm;
	double x = 0, y = 0, z = 0;
	std::ifstream ifs("cube.stl");
	if (ifs.is_open())
		std::cout << "File is Opened" << std::endl;

	while (!ifs.eof())
	{
		getline(ifs, line);

		if (line.find("vertex") != std::string::npos)
		{
			std::istringstream is(line);
			is >> norm >> x >> y >> z;
			vertices.push_back(coordinates( x,y,z ));

			++vx;
		}
	}

	normalCalculation();
	centriodCalculate();
}
/// Calculates the surface normal of each triangular elements drawn using cross product.
///A = ai + bj + ck,
///B = xi + yj + zk
///
///A × B = (bz – cy)i – (az – cx)j + (ay – bx)k
///@param normal unit surface normal for each triangle element.

void Geometry::normalCalculation()
{
	for (unsigned int i = 0; i < vertices.size(); )
	{
		double a[3] = { vertices[i + 1].x - vertices[i].x ,vertices[i + 1].y - vertices[i].y,vertices[i + 1].z - vertices[i].z };

		double b[3] = { vertices[i + 2].x - vertices[i].x ,vertices[i + 2].y - vertices[i].y,vertices[i + 2].z - vertices[i].z };

		double v[3] = { a[1] * b[2] - a[2] * b[1], -(a[0] * b[2] - a[2] * b[0]), a[0] * b[1] - a[1] * b[0] };

		double length = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
		for (int j = 0; j < 3; j++)
		{
			v[j] = v[j] / length;
		}

		normal.push_back(coordinates(v[0], v[1], v[2]));
		nl++;
		i = i + 3;
	}
}
/// Calculates the centroid of the model by calculating the average of all coordinates.
///@param centroid The centroid of the model.
///@see FrameSize(double&,double&,double&,double&,double&,double&).
void Geometry::centriodCalculate()
{
	double minX=0,minY=0,minZ=0,maxX=0,maxY=0,maxZ=0;
	maxX = vertices[0].x; minX = vertices[0].x;
	maxY = vertices[0].y;minY = vertices[0].y;
	maxZ = vertices[0].z;minZ = vertices[0].z;


	for (unsigned int i = 0; i < vertices.size(); i++)
	{	centroid.x += vertices[i].x;
	if ( maxX < vertices[i].x) maxX = vertices[i].x;
	else if( minX > vertices[i].x) minX = vertices[i].x;
	centroid.y += vertices[i].y;
	if ( maxY < vertices[i].y) maxY = vertices[i].y;
	else if( minY > vertices[i].y) minY =  vertices[i].y;
	centroid.z += vertices[i].z;
	if ( maxZ < vertices[i].z) maxZ = vertices[i].z;
	else if( minZ > vertices[i].z) minZ = vertices[i].z;
	}
	centroid.x = centroid.x/vx;
	centroid.y = centroid.y/vx;
	centroid.z = centroid.z/vx;
	FrameSize(minX,minY,minZ,maxX,maxY,maxZ);
}
/// Calculates a cuboidal camera frame w.r.t the size of model.
///	@param maxX,maxY,maxZ maximum value of x,y,z coordinate.
///	@param minX,minY,minZ minimum value of x,y,z coordinate.
/// @param maxDist Distance from the centroid to form a cuboidal frame.

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

ShapeData Geometry::makeStl()
{
	stlpaser();
	ShapeData ret;
	
	ret.numVertices =vertices.size();
	ret.vertices = new Vertex[ret.numVertices];
	for (unsigned int i = 0,n=0; i <ret.numVertices;n++)
	{
		ret.vertices[i].position=glm::vec3(vertices[i].x,  vertices[i].y, vertices[i].z);
		ret.vertices[i].color =glm::vec3(+1.0f, +1.0f, +0.0f);
		ret.vertices[i].normal=glm::vec3(normal[n].x,  normal[n].y, normal[n].z);
		ret.vertices[i+1].position=glm::vec3(vertices[i+1].x,  vertices[i+1].y, vertices[i+1].z);
		ret.vertices[i+1].color =glm::vec3(+1.0f, +1.0f, +0.0f);
		ret.vertices[i+1].normal=glm::vec3(normal[n].x,  normal[n].y, normal[n].z);
		ret.vertices[i+2].position=glm::vec3(vertices[i+2].x,  vertices[i+2].y, vertices[i+2].z);
		ret.vertices[i+2].color =glm::vec3(+1.0f, +1.0f, +0.0f);
		ret.vertices[i+2].normal=glm::vec3(normal[n].x,  normal[n].y, normal[n].z);
		i=i+3;
	}
	

	ret.numIndices = vertices.size();
	ret.indices = new GLushort[ret.numIndices];
	for (unsigned int i = 0; i < (unsigned int)ret.numIndices; i++)
	{ret.indices[i]=i;}
	return ret;
}

ShapeData Geometry::makeTriangle()
{
	ShapeData ret;
	Vertex myTri[] =
	{
		glm::vec3(+0.0f, +1.0f, +0.0f),
		glm::vec3(+1.0f, +0.0f, +0.0f),
		glm::vec3(+0.0f, +0.0f, +1.0f),

		glm::vec3(-1.0f, -1.0f, +0.0f),
		glm::vec3(+0.0f, +1.0f, +0.0f),
		glm::vec3(+0.0f, +0.0f, +1.0f),

		glm::vec3(+1.0f, -1.0f, +0.0f),
		glm::vec3(+0.0f, +0.0f, +1.0f),
		glm::vec3(+0.0f, +0.0f, +1.0f),
	};
	ret.numVertices =sizeof(myTri)/sizeof(*myTri);
	ret.vertices = new Vertex[ret.numVertices];
	memcpy(ret.vertices,myTri,sizeof(myTri));

	GLushort indices[] ={0,1,2};
	ret.numIndices =sizeof(indices)/sizeof(*indices);
	ret.indices = new GLushort[ret.numIndices];
	memcpy(ret.indices,indices,sizeof(indices));
	return ret;

}
	
ShapeData Geometry::makeCube()
{
	ShapeData ret;
	Vertex stackVerts[] =
	{
		glm::vec3(-1.0f, +1.0f, +1.0f),  // 0
		glm::vec3(+1.0f, +0.0f, +0.0f),	// Color
		glm::vec3(+0.0f, +1.0f, +0.0f),  // Normal
		glm::vec3(+1.0f, +1.0f, +1.0f),  // 1
		glm::vec3(+0.0f, +1.0f, +0.0f),	// Color
		glm::vec3(+0.0f, +1.0f, +0.0f),  // Normal
		glm::vec3(+1.0f, +1.0f, -1.0f),  // 2
		glm::vec3(+0.0f, +0.0f, +1.0f),  // Color
		glm::vec3(+0.0f, +1.0f, +0.0f),  // Normal
		glm::vec3(-1.0f, +1.0f, -1.0f),  // 3
		glm::vec3(+1.0f, +1.0f, +1.0f),  // Color
		glm::vec3(+0.0f, +1.0f, +0.0f),  // Normal

		glm::vec3(-1.0f, +1.0f, -1.0f),  // 4
		glm::vec3(+1.0f, +0.0f, +1.0f),  // Color
		glm::vec3(+0.0f, +0.0f, -1.0f),  // Normal
		glm::vec3(+1.0f, +1.0f, -1.0f),  // 5
		glm::vec3(+0.0f, +0.5f, +0.2f),  // Color
		glm::vec3(+0.0f, +0.0f, -1.0f),  // Normal
		glm::vec3(+1.0f, -1.0f, -1.0f),  // 6
		glm::vec3(+0.8f, +0.6f, +0.4f),  // Color
		glm::vec3(+0.0f, +0.0f, -1.0f),  // Normal
		glm::vec3(-1.0f, -1.0f, -1.0f),  // 7
		glm::vec3(+0.3f, +1.0f, +0.5f),  // Color
		glm::vec3(+0.0f, +0.0f, -1.0f),  // Normal

		glm::vec3(+1.0f, +1.0f, -1.0f),  // 8
		glm::vec3(+0.2f, +0.5f, +0.2f),  // Color
		glm::vec3(+1.0f, +0.0f, +0.0f),  // Normal
		glm::vec3(+1.0f, +1.0f, +1.0f),  // 9
		glm::vec3(+0.9f, +0.3f, +0.7f),  // Color
		glm::vec3(+1.0f, +0.0f, +0.0f),  // Normal
		glm::vec3(+1.0f, -1.0f, +1.0f),  // 10
		glm::vec3(+0.3f, +0.7f, +0.5f),  // Color
		glm::vec3(+1.0f, +0.0f, +0.0f),  // Normal
		glm::vec3(+1.0f, -1.0f, -1.0f),  // 11
		glm::vec3(+0.5f, +0.7f, +0.5f),  // Color
		glm::vec3(+1.0f, +0.0f, +0.0f),  // Normal

		glm::vec3(-1.0f, +1.0f, +1.0f),  // 12
		glm::vec3(+0.7f, +0.8f, +0.2f),  // Color
		glm::vec3(-1.0f, +0.0f, +0.0f),  // Normal
		glm::vec3(-1.0f, +1.0f, -1.0f),  // 13
		glm::vec3(+0.5f, +0.7f, +0.3f),  // Color
		glm::vec3(-1.0f, +0.0f, +0.0f),  // Normal
		glm::vec3(-1.0f, -1.0f, -1.0f),  // 14
		glm::vec3(+0.4f, +0.7f, +0.7f),  // Color
		glm::vec3(-1.0f, +0.0f, +0.0f),  // Normal
		glm::vec3(-1.0f, -1.0f, +1.0f),  // 15
		glm::vec3(+0.2f, +0.5f, +1.0f),  // Color
		glm::vec3(-1.0f, +0.0f, +0.0f),  // Normal

		glm::vec3(+1.0f, +1.0f, +1.0f),  // 16
		glm::vec3(+0.6f, +1.0f, +0.7f),  // Color
		glm::vec3(+0.0f, +0.0f, +1.0f),  // Normal
		glm::vec3(-1.0f, +1.0f, +1.0f),  // 17
		glm::vec3(+0.6f, +0.4f, +0.8f),  // Color
		glm::vec3(+0.0f, +0.0f, +1.0f),  // Normal
		glm::vec3(-1.0f, -1.0f, +1.0f),  // 18
		glm::vec3(+0.2f, +0.8f, +0.7f),  // Color
		glm::vec3(+0.0f, +0.0f, +1.0f),  // Normal
		glm::vec3(+1.0f, -1.0f, +1.0f),  // 19
		glm::vec3(+0.2f, +0.7f, +1.0f),  // Color
		glm::vec3(+0.0f, +0.0f, +1.0f),  // Normal

		glm::vec3(+1.0f, -1.0f, -1.0f),  // 20
		glm::vec3(+0.8f, +0.3f, +0.7f),  // Color
		glm::vec3(+0.0f, -1.0f, +0.0f),  // Normal
		glm::vec3(-1.0f, -1.0f, -1.0f),  // 21
		glm::vec3(+0.8f, +0.9f, +0.5f),  // Color
		glm::vec3(+0.0f, -1.0f, +0.0f),  // Normal
		glm::vec3(-1.0f, -1.0f, +1.0f),  // 22
		glm::vec3(+0.5f, +0.8f, +0.5f),  // Color
		glm::vec3(+0.0f, -1.0f, +0.0f),  // Normal
		glm::vec3(+1.0f, -1.0f, +1.0f),  // 23
		glm::vec3(+0.9f, +1.0f, +0.2f),  // Color
		glm::vec3(+0.0f, -1.0f, +0.0f),  // Normal
	};

	ret.numVertices =sizeof(stackVerts)/sizeof(*stackVerts);
	ret.vertices = new Vertex[ret.numVertices];
	memcpy(ret.vertices, stackVerts, sizeof(stackVerts));

	GLushort stackIndices[] = {
		0,   1,  2,  0,  2,  3, // Top
		4,   5,  6,  4,  6,  7, // Front
		8,   9, 10,  8, 10, 11, // Right
		12, 13, 14, 12, 14, 15, // Left
		16, 17, 18, 16, 18, 19, // Back
		20, 22, 21, 20, 23, 22, // Bottom
	};

	ret.numIndices = sizeof(stackIndices)/sizeof(*stackIndices);
	ret.indices = new GLushort[ret.numIndices];
	memcpy(ret.indices, stackIndices, sizeof(stackIndices));

	return ret;
}

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
	ret.indices = new GLushort[ret.numIndices];
	memcpy(ret.indices, stackIndices, sizeof(stackIndices));
	return ret;
}

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

ShapeData Geometry::makePlaneIndices(int dimensions)
{
	ShapeData ret;
	ret.numIndices = (dimensions - 1) * (dimensions - 1) * 2 * 3; // 2 triangles per square, 3 indices per triangle
	ret.indices = new unsigned short[ret.numIndices];
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
ShapeData Geometry::makePlane(int dimensions)
{
	ShapeData ret = makePlaneVerts(dimensions);
	ShapeData ret2 = makePlaneIndices(dimensions);
	ret.numIndices = ret2.numIndices;
	ret.indices = ret2.indices;
	return ret;
}

glm::vec3 Geometry::randomColor()
{
	glm::vec3 ret;
	ret.x = rand() / (float)RAND_MAX;
	ret.y = rand() / (float)RAND_MAX;
	ret.z = rand() / (float)RAND_MAX;
	return ret;
}

ShapeData Geometry::generateNormals(const ShapeData& data)
{
	ShapeData ret;
	ret.numVertices = data.numVertices * 2;
	ret.vertices = new Vertex[ret.numVertices];
	glm::vec3 white(1.0f, 1.0f, 1.0f);
	for (int i = 0; i < data.numVertices; i++)
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
	ret.indices = new GLushort[ret.numIndices];
	for (int i = 0; i < ret.numIndices; i++)
		ret.indices[i] = i;
	return ret;
}