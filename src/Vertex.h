#pragma once
#include "glm.hpp"
///struct to contain a vec3 of position, color & normal of a vertex
struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 normal;
};
