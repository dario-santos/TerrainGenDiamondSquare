#include "Vertex.hpp"

int Vertex::count = 0;

Vertex::Vertex(float x, float y, float z)
{
  this->id = ++count;
  this->point = vec3(x, y, z);
}

Vertex::Vertex(vec3 point)
{
  this->id = ++count;

  this->point = point;
}


void Vertex::SetY(float y)
{
	this->point.y = y;
}

string Vertex::ToString()
{
	int id_up    = up != nullptr ? up->id : -1;
	int id_right = right != nullptr ? right->id : -1;
	int id_down  = down != nullptr ? down->id : -1;
	int id_left  = left != nullptr ? left->id : -1;
	int id_diagonal = diagonal != nullptr ? diagonal->id : -1;

  return "Vertex(" + std::to_string(id) + ", " + std::to_string(point.x) + ", " + std::to_string(point.y) + ", " + std::to_string(point.z) + 
	  "up: "    + std::to_string(id_up) + "," +
	  "right: " + std::to_string(id_right) + "," +
	  "down: "  + std::to_string(id_down) + "," +
	  "left: "  + std::to_string(id_left) + "," +
	  "diagonal: " + std::to_string(id_diagonal)+ ")";
}
