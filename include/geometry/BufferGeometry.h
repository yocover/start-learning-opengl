#ifndef BUFFER_GROMETRY
#define BUFFER_GROMETRY
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>
#include <iostream>

using namespace std;

const float PI = glm::pi<float>();

// mesh.h中也定义此属性
struct Vertex
{
  glm::vec3 Position;  // 顶点位置
  glm::vec3 Normal;    // 法线
  glm::vec2 TexCoords; // 纹理坐标

  glm::vec3 Tangent;   // 切线
  glm::vec3 Bitangent; // 副切线
};

class BufferGeometry
{
public:
  vector<Vertex> vertices;
  vector<unsigned int> indices;
  unsigned int VAO;

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

  // 计算切线向量并添加到顶点属性中
  void computeTangents()
  {
  }

  void dispose()
  {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
  }

private:
  glm::mat4 matrix = glm::mat4(1.0f);

protected:
  unsigned int VBO, EBO;

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
#endif
