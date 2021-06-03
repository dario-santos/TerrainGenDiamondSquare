#pragma once

#include <glm/glm.hpp>
using namespace glm;

#include <string>
using std::string;

class Edge;

class Vertex {
public:
	static int count;

	int id = 0;

	vec3 point;

	Vertex* up    = nullptr;
	Vertex* right = nullptr;
	Vertex* down  = nullptr;
	Vertex* left  = nullptr;
	Vertex* diagonal = nullptr;


	Vertex(float x, float y, float z);

	Vertex(vec3 point);

	void SetY(float y);
  
	string ToString();
};
