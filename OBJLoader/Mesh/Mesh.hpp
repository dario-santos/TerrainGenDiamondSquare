#pragma once

#include <GL/glew.h>

#include <string>
using std::string;

#include <vector>
using std::vector;

#include "Vertex.hpp"
#include "../Shader/GLSLProgram.hpp"

class Mesh {
public:
  int sz = 0;
  Vertex* grid;
  GLSLProgram* shader;

  // Mesh VAO
  GLuint meshVAO = 0;
  GLuint verticeBufferId = 0;

  Mesh(Vertex* grid, int num_faces, GLSLProgram* shader);

  void StoreVBO();

  void Render(glm::mat4& view, glm::mat4& projection);

  string ToString();
};