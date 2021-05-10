#pragma once
#include "primitives/ShapeData.h"
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>
#include<math.h>
#include<cstdlib>
#include<algorithm>
#include<stdio.h>
class coordinates
{
public:
	double x, y, z;
	coordinates():x(0),y(0),z(0){}
	coordinates(double _x,double _y, double _z):x(_x),y(_y),z(_z){}
};

class Geometry: public coordinates
{
private:
	 ShapeData makePlaneVerts(int dimensions);
	 ShapeData makePlaneIndices(int dimensions);
	 glm::vec3 randomColor();
protected:
	std::vector<coordinates> normal;
	std::vector<coordinates> vertices;
	coordinates centroid;
	int nl;
	int vx;
	double FrameRight,FrameTop,FrameFarview,FrameLeft,FrameDown,FrameNearview;

public:
	Geometry();
	~Geometry();
	void stlpaser();
	void normalCalculation();
	void centriodCalculate(); 
	void FrameSize(double&,double&,double&,double&,double&,double&);
	ShapeData makeStl();
	ShapeData makeTriangle();
	ShapeData makeCube();
	ShapeData makeArrow();
	ShapeData makePlane(int dimensions=10);
	ShapeData generateNormals(const ShapeData& data);

};

