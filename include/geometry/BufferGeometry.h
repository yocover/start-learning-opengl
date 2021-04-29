
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>
#include <iostream>

using namespace std;

struct Vertex
{
  glm::vec3 Position;  // 顶点位置
  glm::vec3 Normal;    // 法线
  glm::vec2 TexCoords; // 纹理坐标
};

glm::mat4 _m1 = glm::mat4(1.0f);

class BufferGeometry
{
public:
  vector<Vertex> vertices;
  vector<unsigned int> indices;
  unsigned int VAO;

  void rotateX(float angle)
  {
    _m1 = glm::rotate(_m1, glm::radians(angle), glm::vec3(1.0, 0.0, 0.0));
    this->applyMatrix(_m1);

    this->updateBufferData();
    _m1 = glm::mat4(1.0f);
  }

  void rotateY(float angle)
  {
    _m1 = glm::rotate(_m1, glm::radians(angle), glm::vec3(0.0, 1.0, 0.0));
    this->applyMatrix(_m1);

    this->updateBufferData();
    _m1 = glm::mat4(1.0f);
  }

  void rotateZ(float angle)
  {
    _m1 = glm::rotate(_m1, glm::radians(angle), glm::vec3(0.0, 0.0, 1.0));
    this->applyMatrix(_m1);

    this->updateBufferData();
    _m1 = glm::mat4(1.0f);
  }

  void rotateXYZ(float angle)
  {
    _m1 = glm::rotate(_m1, glm::radians(angle), glm::vec3(1.0, 1.0, 1.0));
    this->applyMatrix(_m1);

    this->updateBufferData();
    _m1 = glm::mat4(1.0f);
  }

  void applyMatrix(glm::mat4 matrix)
  {
    for (unsigned int i = 0; i < vertices.size(); i++)
    {
      vertices[i].Position = matrix * glm::vec4(vertices[i].Position, 1.0f);
    }
  }

  void logParameters()
  {
    for (unsigned int i = 0; i < vertices.size(); i++)
    {
      // cout << "-----------------" << endl;
      // cout << "vertex ->> x: " << vertices[i].Position.x << ",y: " << vertices[i].Position.y << ",z: " << vertices[i].Position.z << endl;
      // cout << "normal ->> x: " << vertices[i].Normal.x << ",y: " << vertices[i].Normal.y << ",z: " << vertices[i].Normal.z << endl;
      // cout << "TexCoords ->> x: " << vertices[i].TexCoords.x << ",y: " << vertices[i].TexCoords.y << endl;
      // cout << "-----------------" << endl;
    }
  }

  void dispose()
  {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
  }

protected:
  unsigned int VBO, EBO;

  void updateBufferData()
  {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(vertices[0]), &vertices[0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  void setupBuffers()
  {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // vertex attribute
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_DYNAMIC_DRAW);

    // indixes
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // 设置顶点属性指针
    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

    // Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));

    // TexCoords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }
};