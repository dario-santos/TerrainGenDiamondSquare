#include "Mesh.hpp"

Mesh::Mesh(Vertex* grid, int numFaces, GLSLProgram* shader)
{
  this->sz = numFaces;
  this->grid = grid;

  this->shader = shader;

  this->StoreVBO();
}

void Mesh::StoreVBO()
{

  std::vector<GLfloat> v;

  int idx = 0;

  auto g = grid;

  while (g->down != nullptr)
  {
      auto A = g;

      while (A->right != nullptr)
      {
        v.push_back(A->point.x);
        v.push_back(A->point.y);
        v.push_back(A->point.z);
        
        v.push_back(A->down->point.x);
        v.push_back(A->down->point.y);
        v.push_back(A->down->point.z);

        v.push_back(A->diagonal->point.x);
        v.push_back(A->diagonal->point.y);
        v.push_back(A->diagonal->point.z);


        v.push_back(A->point.x);
        v.push_back(A->point.y);
        v.push_back(A->point.z);

        v.push_back(A->right->point.x);
        v.push_back(A->right->point.y);
        v.push_back(A->right->point.z);

        v.push_back(A->diagonal->point.x);
        v.push_back(A->diagonal->point.y);
        v.push_back(A->diagonal->point.z);

        A = A->right;
      }

      g = g->down;
    }

  // Move vertex data to video memory; specifically to VBO called vertexbuffer
  glGenVertexArrays(1, &meshVAO);
  glGenBuffers(1, &verticeBufferId);
  glBindVertexArray(meshVAO);

  // Sets the verticeBufferId with the ID of the generated buffer
  glBindBuffer(GL_ARRAY_BUFFER, verticeBufferId);
  // Create a new data store for the buffer object, using the buffer object currently bound to target.
  glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(GLfloat), &v[0], GL_STATIC_DRAW);

  // position attribute
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}


void Mesh::Render(glm::mat4& view, glm::mat4& projection)
{
    glm::mat4 model = glm::mat4(1.0f);
    
    // Uses shaderId as our shader
    GLuint shaderId = shader->getHandle();
    glUseProgram(shader->getHandle());


    // Gets the mvp position
    unsigned int m = glGetUniformLocation(shaderId, "model");
    // Passes the matrix to the shader
    glUniformMatrix4fv(m, 1, GL_FALSE, &model[0][0]);

    unsigned int v = glGetUniformLocation(shaderId, "view");
    // Passes the matrix to the shader
    glUniformMatrix4fv(v, 1, GL_FALSE, &view[0][0]);

    unsigned int p = glGetUniformLocation(shaderId, "projection");
    // Passes the matrix to the shader
    glUniformMatrix4fv(p, 1, GL_FALSE, &projection[0][0]);

    // be sure to activate shader when setting uniforms/drawing objects
    vec3 color = vec3(255.f, 0.0f, 0.0f) / vec3(255.f);
    glUniform3f(glGetUniformLocation(shaderId, "objectColor"), color.x, color.y, color.z);

    glEnableVertexAttribArray(0);

    glBindVertexArray(this->meshVAO);
    glDrawArrays(GL_TRIANGLES, 0, this->sz * 6);

    glDisableVertexAttribArray(0);
}

string Mesh::ToString()
{
  string s = "Mesh = [\n ";

  s += "\n\n\nVertices:\n";

  auto g = grid;

  while (g != nullptr)
  {
    auto A = g;

    while (A != nullptr)
    {
      s += A->ToString() + "\n ";
      
      A = A->right;
    }
    g = g->down;
  }

  return s + "]";
}