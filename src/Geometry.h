#pragma once
#include "ShapeData.h"
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>
#include<math.h>
#include<cstdlib>
#include<algorithm>
#include<stdio.h>
#include <QString>
///stores double xyz coordinates 
class coordinates
{
public:
	double x, y, z;
	coordinates():x(0),y(0),z(0){}
	coordinates(double _x,double _y, double _z):x(_x),y(_y),z(_z){}
};
/// Reads the file ,evaluates the normals,color, centroid and indices of a model
///@param m_Vertices : stores the vertices of the modal
///@param m_Normal	 : stores normal for each vertex
///@param m_VertexCount: total number of vertices
///@param m_NormalCount: total number of normals
class Geometry: public coordinates
{
private:
	 ShapeData makePlaneVerts(int dimensions);
	 ShapeData makePlaneIndices(int dimensions);
	 glm::vec3 randomColor();
	 std::vector<coordinates> m_Vertices;
	 std::vector<coordinates> m_Normal;
	 coordinates m_Centroid;
	 int m_NormalCount;
	 int m_VertexCount;
	 //double FrameRight,FrameTop,FrameFarview,FrameLeft,FrameDown,FrameNearview;

public:
	Geometry();
	~Geometry();
	void stlpaser(const QString& filename);
	void normalCalculation();
	void centriodCalculate(); 
	//void FrameSize(double&,double&,double&,double&,double&,double&);
	ShapeData makeStl(const QString& filepath);
	ShapeData makeArrow();
	ShapeData makePlane(int dimensions=10);
	ShapeData generateNormals(const ShapeData& data);

};

